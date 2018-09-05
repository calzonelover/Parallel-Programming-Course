# Optimization
## HPOP

## Identify hotspot
* cc -pg [source] -o [executable]
* gprof [exec file]

## Memmory transfer in our machine
* Device 0: GeForce GT 1030
 Quick Mode

* Host to Device Bandwidth, 1 Device(s)
 PINNED Memory Transfers
   Transfer Size (Bytes)	Bandwidth(MB/s)
   33554432			1541.5

* Device to Host Bandwidth, 1 Device(s)
 PINNED Memory Transfers
   Transfer Size (Bytes)	Bandwidth(MB/s)
   33554432			1632.4

* Device to Device Bandwidth, 1 Device(s)
 PINNED Memory Transfers
   Transfer Size (Bytes)	Bandwidth(MB/s)
   33554432			13333.1

## Coalesced Access to Global Memory
## Amdahl's Law
* r, T
* Strong scaling <=> Amdahl's law
## Gustafson's law

## Don't forget to check cuda compute error