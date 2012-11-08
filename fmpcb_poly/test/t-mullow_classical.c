/*=============================================================================

    This file is part of ARB.

    ARB is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    ARB is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with ARB; if not, write to the Free Software
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA

=============================================================================*/
/******************************************************************************

    Copyright (C) 2012 Fredrik Johansson

******************************************************************************/

#include "fmpcb_poly.h"


int main()
{
    long iter;
    flint_rand_t state;

    printf("mullow_classical....");
    fflush(stdout);

    flint_randinit(state);

    /* compare with fmpq_poly */
    for (iter = 0; iter < 10000; iter++)
    {
        long qbits1, qbits2, rbits1, rbits2, rbits3, trunc;
        fmpq_poly_t A, B, C;
        fmpcb_poly_t a, b, c, d;

        qbits1 = 2 + n_randint(state, 200);
        qbits2 = 2 + n_randint(state, 200);
        rbits1 = 2 + n_randint(state, 200);
        rbits2 = 2 + n_randint(state, 200);
        rbits3 = 2 + n_randint(state, 200);
        trunc = n_randint(state, 10);

        fmpq_poly_init(A);
        fmpq_poly_init(B);
        fmpq_poly_init(C);

        fmpcb_poly_init(a);
        fmpcb_poly_init(b);
        fmpcb_poly_init(c);
        fmpcb_poly_init(d);

        fmpq_poly_randtest(A, state, 1 + n_randint(state, 10), qbits1);
        fmpq_poly_randtest(B, state, 1 + n_randint(state, 10), qbits2);
        fmpq_poly_mullow(C, A, B, trunc);

        fmpcb_poly_set_fmpq_poly(a, A, rbits1);
        fmpcb_poly_set_fmpq_poly(b, B, rbits2);
        fmpcb_poly_mullow_classical(c, a, b, trunc, rbits3);

        if (!fmpcb_poly_contains_fmpq_poly(c, C))
        {
            printf("FAIL\n\n");
            printf("bits3 = %ld\n", rbits3);
            printf("trunc = %ld\n", trunc);

            printf("A = "); fmpq_poly_print(A); printf("\n\n");
            printf("B = "); fmpq_poly_print(B); printf("\n\n");
            printf("C = "); fmpq_poly_print(C); printf("\n\n");

            printf("a = "); fmpcb_poly_printd(a, 15); printf("\n\n");
            printf("b = "); fmpcb_poly_printd(b, 15); printf("\n\n");
            printf("c = "); fmpcb_poly_printd(c, 15); printf("\n\n");

            abort();
        }

        fmpcb_poly_set(d, a);
        fmpcb_poly_mullow_classical(d, d, b, trunc, rbits3);
        if (!fmpcb_poly_equal(d, c))
        {
            printf("FAIL (aliasing 1)\n\n");
            abort();
        }

        fmpcb_poly_set(d, b);
        fmpcb_poly_mullow_classical(d, a, d, trunc, rbits3);
        if (!fmpcb_poly_equal(d, c))
        {
            printf("FAIL (aliasing 2)\n\n");
            abort();
        }

        fmpq_poly_clear(A);
        fmpq_poly_clear(B);
        fmpq_poly_clear(C);

        fmpcb_poly_clear(a);
        fmpcb_poly_clear(b);
        fmpcb_poly_clear(c);
        fmpcb_poly_clear(d);
    }

    /* check a*(b+c) = a*b+a*c */
    for (iter = 0; iter < 10000; iter++)
    {
        long bits, trunc;
        fmpcb_poly_t a, b, c, bc, abc, ab, ac, abac;

        bits = 2 + n_randint(state, 200);
        trunc = n_randint(state, 10);

        fmpcb_poly_init(a);
        fmpcb_poly_init(b);
        fmpcb_poly_init(c);
        fmpcb_poly_init(bc);
        fmpcb_poly_init(abc);
        fmpcb_poly_init(ab);
        fmpcb_poly_init(ac);
        fmpcb_poly_init(abac);

        fmpcb_poly_randtest(a, state, 1 + n_randint(state, 10), bits, 5);
        fmpcb_poly_randtest(b, state, 1 + n_randint(state, 10), bits, 5);
        fmpcb_poly_randtest(c, state, 1 + n_randint(state, 10), bits, 5);

        fmpcb_poly_add(bc, b, c, bits);
        fmpcb_poly_mullow_classical(abc, a, bc, trunc, bits);

        fmpcb_poly_mullow_classical(ab, a, b, trunc, bits);
        fmpcb_poly_mullow_classical(ac, a, c, trunc, bits);
        fmpcb_poly_add(abac, ab, ac, bits);

        if (!fmpcb_poly_overlaps(abc, abac))
        {
            printf("FAIL\n\n");
            printf("bits = %ld\n", bits);
            printf("trunc = %ld\n", trunc);

            printf("a = "); fmpcb_poly_printd(a, 15); printf("\n\n");
            printf("b = "); fmpcb_poly_printd(b, 15); printf("\n\n");
            printf("c = "); fmpcb_poly_printd(c, 15); printf("\n\n");
            printf("abc = "); fmpcb_poly_printd(abc, 15); printf("\n\n");
            printf("abac = "); fmpcb_poly_printd(abac, 15); printf("\n\n");

            abort();
        }

        fmpcb_poly_clear(a);
        fmpcb_poly_clear(b);
        fmpcb_poly_clear(c);
        fmpcb_poly_clear(bc);
        fmpcb_poly_clear(abc);
        fmpcb_poly_clear(ab);
        fmpcb_poly_clear(ac);
        fmpcb_poly_clear(abac);
    }

    flint_randclear(state);
    _fmpz_cleanup();
    printf("PASS\n");
    return EXIT_SUCCESS;
}
