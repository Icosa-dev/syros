syros_sources = files('kmain.c', 'memory.c')

nasm = find_program('nasm')
nasm_gen = generator(
    nasm,
    output: '@BASENAME@.o',
    arguments: ['-f', 'elf64', '-g', '-F', 'dwarf', '@INPUT@', '-o', '@OUTPUT@'],
)
