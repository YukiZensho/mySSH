#ifndef RSA_H
#define RSA_H

#define rsakey_t unsigned long long

struct rsakeypair{
    rsakey_t p,q;
    rsakey_t privateKey, publicKey;
    rsakey_t n, phi_n;
    rsakey_t e, d;
};

void generate_primes(struct rsakeypair * rsakey){
    rsakeypair.p = 5;
    rsakeypair.q = 7;
    rsakeypair.n = rsakeypair.p * rsakeypair.q;
    rsakeypair.phi_n = (rsakeypair.p
}

#endif
