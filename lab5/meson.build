project(
  'lab5',
  'c',
  default_options : [
    'buildtype=release',
    'debug=false',
    'optimization=3',
    'cpp_std=gnu++23',
    'b_pie=true',
  ]
)

openmp = dependency('openmp', required : true)
threads = dependency('threads')

executable('gol_verify',
  'gol.c',
  'life.c',
  'lifeseq.c',
  'load.c',
  'save.c',
  c_args : '-DVERIFY_FLAG',
  dependencies: [threads],      # Pthreads support.
  install: true
)

executable('gol',
  'gol.c',
  'life.c',
  'lifeseq.c',
  'load.c',
  'save.c',
  c_args : [
    '-march=native',
    '-mtune=native',
    '-finline-functions',
    '-finline-limit=99999',
    '-funroll-loops',
    '-floop-interchange',
    '-floop-block',
    '-floop-parallelize-all',
    '-ftree-vectorize',
    '-fprefetch-loop-arrays',
    '-ffast-math',
    '-fassociative-math',
    '-flto',
    '-fipa-cp-clone',
    '-fomit-frame-pointer',
    '-fno-stack-protector',
    '-fno-asynchronous-unwind-tables',
    '-g0',
    '-fopenmp',
  ],
  link_args : [
    '-flto',
    '-Wl,-O2',
    '-Wl,--gc-sections',
    '-Wl,--as-needed',
    '-lpthread',
    '-fopenmp',
  ],
  dependencies: [threads],      # Pthreads support.
  install: true
)

executable('initboard',
  'bitboard.c',
  'random_bit.c',
)
