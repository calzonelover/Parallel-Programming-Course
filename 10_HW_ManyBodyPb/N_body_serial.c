#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#include "N_body.h"

Particle initParticle(Particle _particles[]);


void main(int argc, char** argv){
    Particle particles[N_PARTICLES];
    float v_half[2], a[2] = {0,0}; // to store data in x, y
    float dr[2], abs_dr[2];
    /// initialize
    *particles = initParticle(particles);
    // for (unsigned int p_i = 0; p_i < N_PARTICLES; p_i++){
    //     printf("Time init Before Particle_%d x = %f y = %f vx=%f vy=%f \n", p_i, particles[p_i].x, particles[p_i].y, particles[p_i].vx, particles[p_i].vy);
    // }
    /// time
    for (unsigned int t_i=0; t_i < NT; t_i++){
        // find acceleration
        for (unsigned int my_i=0; my_i < N_PARTICLES; my_i++){
            for (unsigned int p_i=0; p_i < N_PARTICLES; p_i++){
                if (p_i != my_i){
                    dr[0] = particles[p_i].x - particles[my_i].x;
                    dr[1] = particles[p_i].y - particles[my_i].y;
                    a[0] += particles[p_i].m*dr[0]/pow( dr[0]*dr[0] + dr[1]*dr[1], 3.0f/2.0f);
                    a[1] += particles[p_i].m*dr[1]/pow( dr[0]*dr[0] + dr[1]*dr[1], 3.0f/2.0f);
                }
            }
            particles[my_i].vx += a[0]*STEP_TIME;
            particles[my_i].vy += a[1]*STEP_TIME;
            particles[my_i].x += particles[my_i].vx*STEP_TIME;
            particles[my_i].y += particles[my_i].vy*STEP_TIME;
        }
        a[0] = a[1] = 0.0f;
        snapShot(particles, t_i);
    }
}

Particle initParticle(Particle _particles[]){
    for (unsigned int i = 0; i < N_PARTICLES; i++){
        _particles[i].x = (float) ((rand()/ (float)RAND_MAX - 0.5f)* 2.0f) * (float)X_MAX;
        _particles[i].y = (float) ((rand()/ (float)RAND_MAX - 0.5f)* 2.0f) * (float)X_MAX;
        _particles[i].vx = (float) ((rand()/ (float)RAND_MAX - 0.5f)* 2.0f) * (float)V_MAX;
        _particles[i].vy = (float) ((rand()/ (float)RAND_MAX - 0.5f)* 2.0f) * (float)V_MAX;
        _particles[i].m = MASS;
    }
    return *_particles;
}


void snapShot(Particle _particles[], int _t_i){
	FILE *file;
	char file_name[200];
	sprintf(file_name, "result_serial/Snapshot_t_%d.dat", _t_i);
	file = fopen(file_name, "w");
    for (unsigned int _p_i=0; _p_i<N_PARTICLES; _p_i++){
        fprintf(file, "%f %f\n", _particles[_p_i].x, _particles[_p_i].y);
    }
    fclose(file);
}