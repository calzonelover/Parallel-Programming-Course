program hello
implicit none
!$omp parallel
write(*,*) 'Hello, World!'
!$omp end parallel
end program