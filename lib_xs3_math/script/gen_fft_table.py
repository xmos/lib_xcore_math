# Copyright 2020-2022 XMOS LIMITED.
# This Software is subject to the terms of the XMOS Public Licence: Version 1.
import numpy as np
import argparse
import os


def main():

    parser = argparse.ArgumentParser()

    parser.add_argument(
        "--out_file",
        default="xmath_fft_lut",
        help="Filename to be used (with '.h' and '.c') for the generated files. (default: 'xmath_fft_lut')",
    )
    parser.add_argument(
        "--out_dir", default="./", help="Directory to output generated files to."
    )
    parser.add_argument(
        "--max_fft_log2",
        type=int,
        default=5,
        help="Log2 of the maximum FFT size supported. (default: 5)",
    )
    parser.add_argument(
        "-v",
        "--verbose",
        action="store_true",
        default=False,
        help="Enable verbose mode.",
    )
    parser.add_argument(
        "--dit",
        action="store_true",
        default=False,
        help="Generate decimation-in-time tables.",
    )
    parser.add_argument(
        "--dif",
        action="store_true",
        default=False,
        help="Generate decimation-in-frequency tables.",
    )

    args = parser.parse_args()

    if not (args.dit or args.dif):
        print("Neither the --dit nor --dif flags were provided. No work to be done.")
        return

    header_filename = f"{args.out_file}.h"
    source_filename = f"{args.out_file}.c"
    # header_define = f"{args.out_file.upper()}_H_"

    if args.verbose:
        print(f"Header filename: {header_filename}")
        print(f"Source filename: {source_filename}")

    with open(os.path.join(args.out_dir, source_filename), "w+") as source_file:
        with open(os.path.join(args.out_dir, header_filename), "w+") as header_file:

            header_file.write(
                "// Copyright 2021 XMOS LIMITED. This Software is subject to the terms of the\n"
            )
            header_file.write("// XMOS Public License: Version 1\n")
            header_file.write("#pragma once\n")
            header_file.write('#include "xmath/xmath.h"\n\n')

            source_file.write(f'#include "{header_filename}"\n\n')

            # header.write("#ifndef %s\n"%(header_define))
            # header.write("#define %s\n"%(header_define))

            if args.dit:
                header_file.write(
                    "\n/** @brief Maximum FFT length (log2) that can be performed using decimation-in-time. */\n"
                )
                header_file.write(f"#define MAX_DIT_FFT_LOG2 {args.max_fft_log2}\n")
                header_file.write(
                    "\n/** @brief Convenience macro to index into the decimation-in-time FFT look-up table. \n\n"
                )
                header_file.write(
                    "\tThis will return the address at which the coefficients for the final pass of the real DIT\n"
                )
                header_file.write("\tFFT algorithm begin. \n\n")
                # header_file.write("\t\n")
                header_file.write("\t@param N\tThe FFT length.\n*/\n")
                header_file.write(
                    "#define XMATH_DIT_REAL_FFT_LUT(N) &xmath_dit_fft_lut[(N)-8]\n\n"
                )
            if args.dif:
                header_file.write(
                    "\n/** @brief Maximum FFT length (log2) that can be performed using decimation-in-frequency. */\n"
                )
                header_file.write(f"#define MAX_DIF_FFT_LOG2 {args.max_fft_log2}\n")
                header_file.write(
                    "\n/** @brief Convenience macro to index into the decimation-in-frequency FFT look-up table. \n\n"
                )
                header_file.write(
                    "\tUse this to retrieve the correct address for the DIF FFT look-up table when performing\n"
                )
                header_file.write(
                    "\tan FFT (or IFFT) using the DIF algorithm. (@see fft_dif_forward).\n\n"
                )
                header_file.write("\t@param N\tThe FFT length.\n*/\n")
                header_file.write(
                    "#define XMATH_DIF_FFT_LUT(N) &xmath_dif_fft_lut[(1<<(MAX_DIF_FFT_LOG2)) - (N)]\n\n"
                )

            if args.dit:

                if args.verbose:
                    print("Generating DIT tables..")

                table_name = generate_DIT_FFT(
                    args.max_fft_log2, header_file, source_file, args, M=32
                )

            if args.dif:

                if args.verbose:
                    print("Generating DIF tables..")

                table_name = generate_DIF_FFT(
                    args.max_fft_log2, header_file, source_file, args, M=32
                )


# N #number of rounds
# M #bytes per load
def generate_DIT_FFT(N, header_file, source_file, args, M=8 * 4):

    # First two rounds are done using VFTTF instruction, so twiddle factors are
    # only needed for the remaining rounds.
    twiddle_factor_count = 2 ** (N - 2) - 1
    twiddle_factors = np.zeros((twiddle_factor_count, 4), dtype=np.complex128)

    a = M * 2 ** (N - 3)
    b = M

    p = 0  # data pointer
    t = 0

    table_name = "xmath_dit_fft_lut"

    total_bytes = 4 * twiddle_factor_count * 8
    header_file.write(
        f"extern const complex_s32_t {table_name}[{4*twiddle_factor_count}]; // {total_bytes} bytes\n"
    )

    # a_min = M*2**(N-3) >> (N-2)
    # b_max = M << (N-2)

    # a_values = M * 2**(N - 3 - np.arange(N-2))
    # b_values = M * 2**(np.arange((N-2)))

    # for a, b in zip(a_values, b_values):

    for i in range(N - 2):

        for k in range(b - M, -M, -M):
            s = p + k

            twiddle_offset = (k / M) * (a / M) * 4
            twiddle_stride = 2 ** (N - 3) / (b / M)

            tw = np.arange(4)
            q = twiddle_offset + (twiddle_stride * tw)
            twiddle_factors[t, :] = np.exp(-2.0j * np.pi * q / (2 ** N))

            if args.verbose:
                tmp = "\t".join([str(asd) for asd in q])
                print(f"load twiddle\t{t}[ {tmp}]")

            t += 1

            for j in range(a - M, -M, -M):

                if args.verbose:
                    # print ("(%06x %06x)" %(s, s + b)),
                    print(f"({s} {s+b})", end="")

                s += 2 * b

            if args.verbose:
                print("")

        b *= 2
        a //= 2

        if args.verbose:
            print("------")

    twiddle_exponent = -30
    r = twiddle_factors.view(np.float64).flatten()
    twiddle_factors_int = np.asarray(
        np.rint(r * 2 ** -twiddle_exponent), dtype=np.int32
    )

    twiddle_factors_int = np.reshape(twiddle_factors_int, (twiddle_factor_count * 4, 2))

    source_file.write(
        f"const complex_s32_t {table_name}[{4*twiddle_factor_count}] = \n\t{{\n\t"
    )

    for t in range(len(twiddle_factors_int)):
        source_file.write(
            "{%11d, %11d}, " % (twiddle_factors_int[t][0], twiddle_factors_int[t][1])
        )
        if (t + 1) % 4 == 0:
            source_file.write("\n\t")

    source_file.write("};\n\n")

    source_file.write(f"const uint32_t {table_name}_size = sizeof({table_name});\n\n")

    return table_name


def generate_DIF_FFT(N, header_file, source_file, args, M=8 * 4):

    twiddle_factor_count = 2 ** (N - 2) - 1
    twiddle_factors = np.zeros((twiddle_factor_count, 4), dtype=np.complex128)

    a = M
    b = M * 2 ** (N - 3)  # first two rounds are done by the fft inst

    p = 0  # data pointer
    t = 0

    table_name = "xmath_dif_fft_lut"

    total_bytes = 4 * twiddle_factor_count * 8

    header_file.write(
        f"extern const complex_s32_t {table_name}[{4*twiddle_factor_count}]; // {total_bytes} bytes\n"
    )

    for i in range(N - 2):

        for k in range(b - M, -M, -M):
            # for k in range(0, b, M):
            s = p + k

            twiddle_offset = (k / M) * (a / M) * 4
            twiddle_stride = 2 ** (N - 3) / (b / M)

            if args.verbose:
                print(f"load twiddle\t{t}[ ", end="")

            for tw in range(4):
                q = twiddle_offset + (twiddle_stride * tw)
                twiddle_factors[t, tw] = np.exp(-2.0j * np.pi * q / 2 ** N)

                if args.verbose:
                    print(f"{q}\t", end="")

            if args.verbose:
                print("]")

            t += 1

            for j in range(a - M, -M, -M):

                if args.verbose:
                    print(f"({s} {s+b})", end="")

                s += 2 * b

            if args.verbose:
                print("")

        b //= 2
        a *= 2

        if args.verbose:
            print("------")

    twiddle_exponent = -30
    r = twiddle_factors.view(np.float64).flatten()
    twiddle_factors_int = np.asarray(
        np.rint(r * 2 ** -twiddle_exponent), dtype=np.int32
    )

    twiddle_factors_int = np.reshape(twiddle_factors_int, (twiddle_factor_count * 4, 2))

    # Let's make them negative to invert the loading order in asm
    twiddle_factors_int = -twiddle_factors_int

    source_file.write(
        f"const complex_s32_t {table_name}[{4*twiddle_factor_count}] = \n\t{{\n\t"
    )

    for t in range(len(twiddle_factors_int)):
        source_file.write(
            "{%11d, %11d}, " % (twiddle_factors_int[t][0], twiddle_factors_int[t][1])
        )
        if (t + 1) % 4 == 0:
            source_file.write("\n\t")

    source_file.write("};\n\n")

    source_file.write(f"const uint32_t {table_name}_size = sizeof({table_name});\n\n")

    return table_name


if __name__ == "__main__":
    main()

