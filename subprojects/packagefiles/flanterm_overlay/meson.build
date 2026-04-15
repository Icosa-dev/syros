project('flanterm', 'c', version: '1.0')

flanterm_inc = include_directories('src')

flanterm_sources = files(
    'src/flanterm.c',
    'src/flanterm_backends/fb.c'
)

flanterm_dep = declare_dependency(
    include_directories: flanterm_inc,
    sources: flanterm_sources
)
