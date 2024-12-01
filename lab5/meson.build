project(
  'lab5',
  'c',
  default_options : [
    'buildtype=plain',
    'debug=false',
    'optimization=3',
    'cpp_std=gnu++23',
    'b_pie=true',
  ]
)

openmp = dependency('openmp', required : true)

executable('gol_verify',
  'gol.c',
  'life.c',
  'lifeseq.c',
  'load.c',
  'save.c',
  c_args : '-DVERIFY_FLAG',
  dependencies : openmp,
)

executable('gol',
  'gol.c',
  'life.c',
  'lifeseq.c',
  'load.c',
  'save.c',
  dependencies : openmp,
)

executable('initboard',
  'bitboard.c',
  'random_bit.c',
)
