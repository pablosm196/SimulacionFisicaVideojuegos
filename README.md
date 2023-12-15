# Sólidos Rígidos

He creado un sólido estático en el main que funciona como suelo.

Creo una nueva clase dentro de Particle.h llamada RigidParticle que hereda de Particle y tiene una escena, un tipo PxRigidDynamic y una referencia al generador que lo crea. Esta clase hereda de Particle para poder aprovechar la funcionalidad que ya tenía implementada de las fuerzas para el resto de partículas. Como del movimiento y la aplicación de físicas ya se encarga el motor, su método integrate solo se encarga de sumarle el tiempo que lleva viva la partícula.

Finalmente creo una clase GaussianRigidGenerator, la cual hereda de GaussianGenerator. Tiene dos variables que indican el máximo de partículas que puede tener ese generador y el número de partículas actuales. En el ParticleSystem hay un generador de este tipo.
