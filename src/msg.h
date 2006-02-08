// -*- c++ -*-
// $Id$
/*
 * Copyright 2003-2004 Spencer Eugene Olson --- All Rights Reserved
 *
 * $Log$
 *
 */

/** \file
 * Generic c++ classes to help sending arbitrarily sized and typed messages via IPC.
 *
 * I'd be really excited if anyone ever used this stuff.
 *
 * Copyright 2003-2004 Spencer Eugene Olson --- All Rights Reserved
 * 
 * */

#ifndef MSG_H
#define MSG_H

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <stdexcept>
#include <string.h>
#include <errno.h>


/** sent to the receiver to ensure existence. */
#define PING_MSG        1
#define PRIV_SERVER_MSG (PING_MSG+1)
#define PRIV_TO_MSG     PRIV_SERVER_MSG
/** reply from the receiver. */
#define PONG_MSG        (1<<24) /* just some big number--could be bigger--anything less than (1<<32)-1 */

#define PRIV_CLIENT_MSG (PONG_MSG+1)
#define PRIV_FROM_MSG   PRIV_CLIENT_MSG

#ifndef MSGMAX
#define MSGMAX 8192
#endif


/** returns ta-tb converted to milliseconds.
 * */
inline float timeDiffMS(struct timeval &ta, struct timeval &tb) {
    return (1e+3 * (ta.tv_sec - tb.tv_sec))
        +  (1e-3 * (ta.tv_usec - tb.tv_usec));
}

/** A class for sending receiving messages to control the LiXon program. */
template < long _mtype = 0, typename T = unsigned char >
class IPCMessage {
  public:
    inline IPCMessage() : mtype(_mtype), msize(sizeof(T)) {} 
  private:
    long mtype;
  public:
    struct message_s {
        /** This is just a copy of mtype and only valid during
         * IPCMessage::send and IPCMessage::receive.
         * */
        long type;
        /** Timestamp of when the message was sent. */
        struct timeval timestamp;
        unsigned char data[MSGMAX - sizeof(struct timeval)];
    } __attribute__ ((packed)) message;
  
    /** Timestamp of when the message was received. */
    struct timeval rtimestamp;
  protected:
    /** Indicates the length of the message data in message.data;
     * Not sent to the message queue; only used to indicate how large a
     * message is to be sent to the message queue.
     * */
    size_t msize;
  public:
    void setSize() { msize = sizeof(T);}
    void setType() { mtype = _mtype; }

    inline const long & type() const { return mtype; }

    inline T & value() { return value<T>(); }
    inline const T & value() const { return value<T>(); }

    template <typename TT>
    inline TT & value() { return *((TT*) (this->message.data) ); }
    template <typename TT>
    inline const TT & value() const { return *((TT*) (this->message.data)); }

    /** Receive an IPC message in normal generic server mode.
     * @param msqid
     *     The message queue ID as returned by getMsgQID().
     *
     * @param buf
     *     A user supplied buffer to read the message into; the function will
     *     allocate a buffer if none is passed in (the user is responsible for
     *     freeing this memory.)
     *
     * @param msgflag
     *     IPC msgflg parameter for msgrcv (see man page for msgrcv)
     *     [Default IPC_NOWAIT].
     *
     * @returns NULL if no message of requested type is available, else
     *     returns a pointer to an IPCMessage.
     *
     * @throws std::runtime_error with what() set to strerror(errno) if there
     *     is an error other than ENOMSG.
     *
     * @see IPCMessage::receive.
     * */
    static IPCMessage * sreceive(int msqid, IPCMessage * buf = NULL, int msgflag = IPC_NOWAIT) {
        return receive(msqid, buf, (1-PONG_MSG), msgflag);
    }

    /** Receive an IPC message from the message queue.
     * This function will attempt to receive any type of message.
     *
     * What ever is returned by this function, the user is responsible to
     * assure that the memory is freed when finished.
     * @param msqid
     *     The message queue ID as returned by getMsgQID().
     *
     * @param buf
     *     A user supplied buffer to read the message into; the function will
     *     allocate a buffer if none is passed in (the user is responsible for
     *     freeing this memory.)
     *
     * @param msgtype
     *     Specify the type of message that is to be returned; This defaults
     *     to returning the next message of any type from the message queue.
     *
     * @param msgflag
     *     IPC msgflg parameter for msgrcv (see man page for msgrcv) [Default 0].
     *
     * @returns NULL if no message of requested type is available, else
     *     returns a pointer to an IPCMessage.
     *
     * @throws std::runtime_error with what() set to strerror(errno) if there
     *     is an error other than ENOMSG.
     * */
    static IPCMessage * receive(int msqid,
                                IPCMessage * buf = NULL,
                                long msgtype = _mtype,
                                int msgflag = 0) {
        if (!buf) {
            /* user didn't allocate a buffer, so we will. */
            buf = new IPCMessage;
        } /* else use the users pre-allocated buffer. */

        ssize_t s = msgrcv(msqid,
                          &buf->message,
                          sizeof(buf->message) - sizeof(buf->mtype),
                          msgtype,
                          msgflag);

        buf->mtype = buf->message.type;

        if (s < 0) {
            if (errno == ENOMSG) return NULL;
            else throw std::runtime_error(strerror(errno));
        }

        /* set the received timestamp. */
        gettimeofday(&buf->rtimestamp,NULL);

        return buf;
    }

    /** Send this IPCMessage to the message queue.
     * @param msqid
     *     The message queue ID as returned by getMsgQID().
     *
     * @param msgflag
     *     IPC msgflg parameter for msgsnd (see man page for msgsnd) [Default 0].
     *
     * @throws std::runtime_error with what() set to strerror(errno) if.
     * */
    inline void send (int msqid, int msgflag = 0) {
        gettimeofday(&message.timestamp,NULL);
        this->message.type = this->mtype;
        int s = msgsnd(msqid, &this->message, this->size(), msgflag);
        if (s < 0) throw std::runtime_error(strerror(errno));
    }

    inline size_t size() const { return sizeof(struct timeval) + this->msize; }

    /** The time difference between message sent and received.
     * @returns
     *     The traversal time in milliseconds.
     * */
    inline float traversalTimeMS() {
        return timeDiffMS(rtimestamp, message.timestamp);
    }

    static float ping(int msqid);
};

typedef IPCMessage<PING_MSG> PingMessage;
typedef IPCMessage<PONG_MSG> PongMessage;

/** ping the receiver to see if it is there.
 * Currently this will wait until the other side replies.
 * FIXME:  add an alarm to wake this thread up so we can timeout.
 *
 * @returns Total time waited for ping.
 * */
template < unsigned char _mtype, typename T >
float IPCMessage<_mtype,T>::ping(int msqid) {
    PingMessage buf;
    struct timeval starttime;
    gettimeofday(&starttime, NULL);
    buf.send(msqid);

    float time = 0;
    do {
        PongMessage::receive(msqid, (PongMessage*)&buf);
        /* ignore pong messages sent before this request. */
    } while ( (time = timeDiffMS(buf.message.timestamp, starttime)) < 0.0 );
    return time;
}

inline int getMsgQID(const std::string & path, char proj_id) {
    key_t msg_key = ftok(path.c_str(), proj_id);

    int msqid = msgget(msg_key, IPC_CREAT|S_IRWXU);

    if (msqid <=0) {
        throw std::runtime_error(std::string("error obtaining message queue\n") + strerror(errno));
    }

    return msqid;
}

#endif // MSG_H
