# OpenMP

* Using shared folder for compile mpi program across domain
* Explicit synchronization: "omp critical" section (as atomic operation for avoid data race)
* Load balancing: scheduling (Slide p.29)
    * simple static
    * Interleave: round-robin => sequentially job send
    * Simple dynamic: aync master-slave like, also have chunk size
    * guided self-schedule: could decrease chunk size
    * Runtime ? >> export OMP_SCHEDULE='dynamic,3'