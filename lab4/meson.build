project(
  'lab4',
  'cpp',
  default_options : [
    'buildtype=plain',
    'debug=false',
    'optimization=2',
    'cpp_std=gnu++23',
    'b_pie=true',
  ]
)

# Dependency for pthreads (POSIX threads)
threads = dependency('threads')

# Executable for each version of randtrack
executable('randtrack_global_lock',
  'randtrack_global_lock.cpp',
  dependencies: [threads],      # Pthreads support.
  install: true
)

executable('randtrack_list_lock',
  'randtrack_list_lock.cpp',
  dependencies: [threads],      # Pthreads support.
  install: true
)

executable('randtrack',
  'randtrack.cpp',
)

executable('randtrack_tm',
  'randtrack_TM.cpp',
  cpp_args: ['-fgnu-tm'],       # Transactional memory flag.
  link_args: ['-litm'], 
  dependencies: [threads],      # Pthreads support.
  install: true
)

executable('randtrack_element_lock',
  'randtrack_element_lock.cpp',
  dependencies: [threads],      # Pthreads support.
  install: true
)

executable('randtrack_reduction',
  'randtrack_reduction.cpp',
  dependencies: [threads],      # Pthreads support.
  install: true
)
