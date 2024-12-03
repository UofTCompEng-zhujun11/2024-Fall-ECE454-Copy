project(
  'lab5',
  'c',
  default_options : [
    'buildtype=plain',
    'debug=false',
    'optimization=2',
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
  dependencies: [threads],      # Pthreads support.
  install: true
)

executable('initboard',
  'bitboard.c',
  'random_bit.c',
)
