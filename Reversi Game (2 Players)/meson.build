project('lab8part1', 'c',
  default_options : ['c_std=gnu18', 'warning_level=3'],
)

exe = executable(
  'part1', 
  'part1.c', 
  include_directories: include_directories('starter-code'), 
  link_args: '-lm'
)

subdir('tests')
