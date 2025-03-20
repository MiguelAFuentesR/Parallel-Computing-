#ifdef GALAX_MODEL_CPU_FAST

#include <cmath>

#include "Model_CPU_fast.hpp"

#include <xsimd/xsimd.hpp>
#include <omp.h>

namespace xs = xsimd;
using b_type = xs::batch<float, xs::avx2>;

Model_CPU_fast
::Model_CPU_fast(const Initstate& initstate, Particles& particles)
: Model_CPU(initstate, particles)
{
}


void Model_CPU_fast
::step()
{


    std::fill(accelerationsx.begin(), accelerationsx.end(), 0);
    std::fill(accelerationsy.begin(), accelerationsy.end(), 0);
    std::fill(accelerationsz.begin(), accelerationsz.end(), 0);

    // // Paralelización con OpenMP
    // #pragma omp parallel for schedule(dynamic)
    // for (int i = 0; i < n_particles; i++) {
    //     float ax = 0.0f, ay = 0.0f, az = 0.0f;

    //      for (int j = 0; j < n_particles; j++) {
    //         b_type diffx = xs::load_unaligned(&particles.x[j]) - particles.x[i];
    //         // b_type diffy = xs::load_unaligned(&particles.y[j]) - particles.y[i];
    //        	// b_type diffz = xs::load_unaligned(&particles.z[j]) - particles.z[i];

    //     //    b_type dij = diffx * diffx + diffy * diffy + diffz * diffz;
    //     //    auto mask = dij < b_type(1.0f);
    //     //    dij = xs::select(mask, b_type(10.0f), b_type(10.0f) / (xs::sqrt(dij) * dij * dij));

    //     //    ax += xs::hadd(diffx * dij * xs::load_unaligned(&initstate.masses[j]));
    //     //    ay += xs::hadd(diffy * dij * xs::load_unaligned(&initstate.masses[j]));
    //     //    az += xs::hadd(diffz * dij * xs::load_unaligned(&initstate.masses[j]));
    //      }

    //     #pragma omp atomic
    //     accelerationsx[i] += ax;
    //     #pragma omp atomic
    //     accelerationsy[i] += ay;
    //     #pragma omp atomic
    //     accelerationsz[i] += az;
    // }

    // // Actualización de posiciones y velocidades
    // #pragma omp parallel for
    // for (int i = 0; i < n_particles; i++) {
    //     velocitiesx[i] += accelerationsx[i] * 2.0f;
    //     velocitiesy[i] += accelerationsy[i] * 2.0f;
    //     velocitiesz[i] += accelerationsz[i] * 2.0f;

    //     particles.x[i] += velocitiesx[i] * 0.1f;
    //     particles.y[i] += velocitiesy[i] * 0.1f;
    //     particles.z[i] += velocitiesz[i] * 0.1f;
    // }
	

//OMP  version
//#pragma omp parallel for schedule(static,1)
#pragma omp parallel for 
    for (int i = 0; i < n_particles; i++) {
        for (int j = 0; j < n_particles; j++) {
            if(i != j)
			{
				const float diffx = particles.x[j] - particles.x[i];
				const float diffy = particles.y[j] - particles.y[i];
				const float diffz = particles.z[j] - particles.z[i];

				float dij = diffx * diffx + diffy * diffy + diffz * diffz;

				if (dij < 1.0)
				{
					dij = 10.0;
				}
				else
				{
					dij = std::sqrt(dij);
					dij = 10.0 / (dij * dij * dij);
				}

				accelerationsx[i] += diffx * dij * initstate.masses[j];
				accelerationsy[i] += diffy * dij * initstate.masses[j];
				accelerationsz[i] += diffz * dij * initstate.masses[j];
			}
        }
    }
  #pragma omp parallel for
  for (int i = 0; i < n_particles; i++)
  {
		  velocitiesx[i] += accelerationsx[i] * 2.0f;
		  velocitiesy[i] += accelerationsy[i] * 2.0f;
		  velocitiesz[i] += accelerationsz[i] * 2.0f;
		  particles.x[i] += velocitiesx   [i] * 0.1f;
		  particles.y[i] += velocitiesy   [i] * 0.1f;
		  particles.z[i] += velocitiesz   [i] * 0.1f;
  }


}
#endif // GALAX_MODEL_CPU_FAST
