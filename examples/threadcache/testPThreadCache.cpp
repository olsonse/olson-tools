#include <olson-tools/PThreadCache.h>

#include <iostream>
#include <cmath>

struct Task : olson_tools::PThreadTask {
  /* MEMBER STORAGE */
  double xi, xf, dx;
  double retval;

  /* MEMBER FUNCTIONS */
  Task(const double & xi, const double & xf, const double dx)
    : xi(xi), xf(xf), dx(dx), retval(0) {}
  virtual ~Task() {}
  virtual void exec() {
    for ( double x = xi; x <= xf; x+= dx ) {
      retval += std::log(x);
    }
  };
};

int main() {
  using olson_tools::pthreadCache;
  using olson_tools::PThreadTaskSet;
  //pthreadCache.set_max_threads(2);
  PThreadTaskSet tasks;

  double dx = 3e-2;
  for (double i = 1; i< 10.0; i+= dx) {
    Task * t = new Task(i, i+dx, dx*1e-5);
    tasks.insert(t);
    pthreadCache.addTask(t);
  }

  std::cout << "queued " << tasks.size() << " tasks" << std::endl;

  int n_finished = 0;
  double sum = 0;
  while ( tasks.size() ) {
    PThreadTaskSet finished = pthreadCache.waitForTasks(tasks);
    n_finished += finished.size();

    for ( PThreadTaskSet::iterator i = finished.begin();
          i != finished.end(); ++i ) {
      Task * t = static_cast<Task*>(*i);
      sum += t->retval;
      delete t;
    }

    PThreadTaskSet tmp;
    std::set_difference(tasks.begin(), tasks.end(),
                        finished.begin(), finished.end(),
                        inserter(tmp, tmp.begin()));
    tasks.swap(tmp);
  }

  std::cout << "sum:  " << sum << "\n"
               "finished " << n_finished << " tasks" << std::endl;

  return EXIT_SUCCESS;
}
