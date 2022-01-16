#ifndef MAPTEL_H
#define MAPTEL_H

#ifdef __cplusplus
#include <iostream>
#include <cstdio>
#else
#include <stdio.h>
#endif

#ifdef __cplusplus
namespace jnp1 {
    extern "C" {
#endif

/*
 * Makro opisujące maksymalną długość nr-u telefonu
 */
    static const size_t TEL_NUM_MAX_LEN = 22;

/**
 * Tworzy słownik i zwraca liczbę naturalną będącą jego identyfikatorem.
  unsigned long maptel_create(void);
  @return identyfikator
 */
    unsigned long maptel_create(void);

/**
 * Usuwa słownik o identyfikatorze id.
 * @param id
 */
    void maptel_delete(unsigned long id);

/**
 * Wstawia do słownika o identyfikatorze id informację o zmianie numeru
 * tel_src na numer tel_dst. Nadpisuje ewentualną istniejącą informację.
 * @param id identyfikator
 * @param tel_src numer początkowy
 * @param tel_dst numer docelowy
 */
    void maptel_insert(unsigned long id, char const *tel_src,
                      char const *tel_dst);

/**
 * Jeśli w słowniku o identyfikatorze id jest informacja o zmianie numeru
 * tel_src, to ją usuwa. W przeciwnym przypadku nic nie robi.
 * @param id identyfikator
 * @param tel_src numer początkowy
 */
    void maptel_erase(unsigned long id, char const *tel_src);

/**
 * Sprawdza, czy w słowniku o identyfikatorze id jest zapisana zmiana numeru
 * tel_src. Podąża ciągiem kolejnych zmian. Zapisuje zmieniony numer w tel_dst.
 * Jeśli nie ma zmiany numeru lub zmiany tworzą cykl, to zapisuje w tel_dst
 * numer tel_src. Wartość len to rozmiar przydzielonej pamięci wskazywanej
 * przez tel_dst.
 * @param id identyfikator
 * @param tel_src numer początkowy
 * @param tel_dst numer docelowy
 * @param len rozmiar przydzielonej pamięci wskazywanej przez tel_dst
 */
    void maptel_transform(unsigned long id, char const *tel_src, char *tel_dst,
                          size_t len);

#ifdef __cplusplus
    }
}
#endif
#endif

