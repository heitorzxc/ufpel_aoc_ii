#!/bin/bash
# <name>:<nsets>:<bsize>:<assoc>:<repl>

# cache de dados níveis 1 e 2
dados1="-cache:dl1 dl1:16:64:1:r"
dados2="-cache:dl2 none"

# cache de instruções níveis 1 e 2
instr1="-cache:il1 il1:16:64:1:r"
instr2="-cache:il2 none"

# TLB de dados e instruções (ausentes)
tlb1="-tlb:dtlb none"
tlb2="-tlb:itlb none"

# GCC_1:
./sim-cache $dados1 $dados2 $instr1 $instr2 $tlb1 $tlb2 cc1.ss gcc.i
# GCC_2:
./sim-cache $dados1 $dados2 $instr1 $instr2 $tlb1 $tlb2 cc1.ss cccp.i
# GCC_3:
./sim-cache $dados1 $dados2 $instr1 $instr2 $tlb1 $tlb2 cc1.ss stmt.i
# GCC_4:
./sim-cache $dados1 $dados2 $instr1 $instr2 $tlb1 $tlb2 cc1.ss jump.i
# GO_1:
./sim-cache $dados1 $dados2 $instr1 $instr2 $tlb1 $tlb2 go.ss 50 9 2stone9.in
# LI_1:
./sim-cache $dados1 $dados2 $instr1 $instr2 $tlb1 $tlb2 li.ss li.lsp
# LI_2:
./sim-cache $dados1 $dados2 $instr1 $instr2 $tlb1 $tlb2 li.ss queen6.lsp
# LI_3:
./sim-cache $dados1 $dados2 $instr1 $instr2 $tlb1 $tlb2 li.ss train.lsp
# PERL_1:
./sim-cache $dados1 $dados2 $instr1 $instr2 $tlb1 $tlb2 perl.ss scrabbl.pl < scrabbl.in
# VORTEX_1:
./sim-cache $dados1 $dados2 $instr1 $instr2 $tlb1 $tlb2 vortex.ss vortex.in
# VORTEX_2:
./sim-cache $dados1 $dados2 $instr1 $instr2 $tlb1 $tlb2 vortex.ss tiny.in
# IJPEG_1:
./sim-cache $dados1 $dados2 $instr1 $instr2 $tlb1 $tlb2 ijpeg.ss -image_file tinyrose.ppm -compression.quality 90 -compression.optimize_coding 0 -compression.smoothing_factor 90 -difference.image 1 -difference.x_stride 10 -difference.y_stride 10 -verbose 1 -GO.findoptcomp
# IJPEG_2:
./sim-cache $dados1 $dados2 $instr1 $instr2 $tlb1 $tlb2 ijpeg.ss -image_file specmun.ppm -compression.quality 90 -compression.optimize_coding 0 -compression.smoothing_factor 90 -difference.image 1 -difference.x_stride 10 -difference.y_stride 10 -verbose 1 -GO.findoptcomp
# IJPEG_3:
./sim-cache $dados1 $dados2 $instr1 $instr2 $tlb1 $tlb2 ijpeg.ss -image_file ijpeg.ppm -compression.quality 90 -compression.optimize_coding 0 -compression.smoothing_factor 90 -difference.image 1 -difference.x_stride 10 -difference.y_stride 10 -verbose 1 -GO.findoptcomp