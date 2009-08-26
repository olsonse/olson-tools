#include <olson-tools/PThreadCache.h>

#include <iostream>
#include <cmath>

struct Task : olson_tools::PThreadTask {
  /* MEMBER STORAGE */
  double x;
  double retval;

  /* MEMBER FUNCTIONS */
  Task(const double & x) : x(x), retval(0) {}
  virtual ~Task() {}
  virtual void exec() {
    retval = std::log(x);
  };
};

int main() {
  pthreadCache.set_max_threads(2);
  PThreadTaskSet tasks;

  for (double i = 0; i< 1.0; i+= 3e-8) {
    Task * t = new Task(i);
    tasks.insert(t);
    pthreadCache.addTask(t);
  }

  double sum = 0;
  while ( tasks.size() ) {
    PThreadTaskSet finished = pthreadCache.waitForTasks(tasks);

    for ( PThreadTaskSet::iterator i = finished.begin();
          i != finished.end(); ++i ) {
      sum += (*i)->retval;
      delete (*i);
    }

    PThreadTaskSet tmp;
    std::set_difference(tasks.begin(), tasks.end(),
                        finished.begin(), finished.end(),
                        inserter(tmp, tmp.begin()));
    tasks.swap(tmp);
  }

  std::cout << "sum:  " << sum << std::endl;

  return EXIT_SUCCESS;
}
