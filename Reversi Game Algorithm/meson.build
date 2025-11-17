project('lab8part2', 'c',
  default_options : ['c_std=gnu18', 'warning_level=3'],
)

exe = executable(
  'part2', 
  ['part2-main.c', 'part2.c'], 
  include_directories: include_directories('starter-code'), 
  link_args: '-lm'
)

subdir('tests')
