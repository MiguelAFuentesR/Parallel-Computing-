# 7 Mars

Afin d'optimiser le code proposé, nous avons décidé de tester les différents types de parallélisation vus pendant le cours dans l'ordre suivant 


1. parallélisation au niveau des threads (TPL avec OMP)
2. Au niveau des données xsimd
3. Au niveau du GPU (cuda)
4. Au niveau du nœud (NPL) 

Nous avons d'abord compilé le projet, étudié le code et évalué les performances de base sur les ordinateurs suivants : 

* Personal : 

CPU: 11th Gen Intel i5-1135G7 (8) @ 4.200GHz 

GPU: Intel TigerLake-LP GT2 [Iris Xe Graphics] 

ram : 12 Gb

Caches (somme de toutes) :           

  L1d :                                     192 KiB (4 instances)
  L1i :                                     128 KiB (4 instances)
  L2 :                                      5 MiB (4 instances)
  L3 :                                      8 MiB (1 instance)

* Ecole :

CPU: Intel(R) Core(TM) i5-6500 @ 3.20GHz
GPU: NVIDIA GeForce GTX 1080 8GB
ram : 16 Gb

Caches (somme de toutes) :                  
  L1d :                                     128 KiB (4 instances)
  L1i :                                     128 KiB (4 instances)
  L2 :                                      1 MiB (4 instances)
  L3 :                                      6 MiB (1 instance)



### Fonctionnement sans optimisation


Con el naive method y con 10000 particulas el codigo permite alcanzar  los siguientes resultados : 

* Personal : 


./bin/galax --display SDL2 -n 1000   ---- > quite updates per second: 32.6509

./bin/galax --display SDL2 -n 10000  ---- > quite updates per second: 0


* Ecole : 

Sans Display : 

./bin/galax  -n 1000 ---- > quite updates per second: 143.885
./bin/galax  -n 5000 ---- > quite updates per second: 7.6
./bin/galax  -n 10000 ---- > quite updates per second: 1.96

Realizando la paralelizacion con OMP se consigue : 
 

* Personal : 
./bin/galax -c CPU_FAST -n 1000 --display SDL2
quite updates per second: 109.5

-n 5000 :   quite updates per second:  3.12602
-n 10000 :   quite updates per second: 2.09232



* Ecole : 


De igual manera hemos probado con las  flags for schedule(static), flags for schedule(static,1) y no cambia el resultado considerablemente

./bin/galax -c CPU_FAST --display NO -n 10000
State updates per second: 0.590267


En el ordenador de la escuela  sin ningun programa abierto y en promedio con 16% de consumacion de CPU para el proceso galax: 


./bin/galax --display SDL2 -n 1000   ---- > quite updates per second: 176.703
./bin/galax --display SDL2 -n 5000   ---- > quite updates per second: 8.29

* Codigo empleado : 
```
OMP  version
#pragma omp parallel for schedule(static,1)
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

```









# 14 Mars

Empezamos con XSIMD para hacer vectorizacion, en un primer acercamiento mantenemos la primer parte del codigo y hemos modificado la ultima 



# 20 Mars




