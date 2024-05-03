import numpy as np
import audio_dsp.dsp.cascaded_biquads as cbq
import audio_dsp.dsp.utils as utils


def saturation_test(filter: cbq.cascaded_biquads_8, fs):

    signal = 2**(np.arange(0, 31.5, 0.5)) - 1
    signal = np.repeat(signal, 2)
    signal[::2] *= -1

    # used for lib_xcore_math biquad test
    sigint = (np.round(signal).astype(np.int32))
    np.savetxt("sig.csv", sigint, fmt="%i", delimiter=",")

    signal *= (2**-31)

    output_int = np.zeros(len(signal))
    output_flt = np.zeros(len(signal))
    output_vpu = np.zeros(len(signal))

    filter.reset_state()
    for n in np.arange(len(signal)):
        output_vpu[n] = filter.process_xcore(signal[n])

    # reference result for lib_xcore_math test
    vpu_int = (np.round(output_vpu * 2**31).astype(np.int32))
    np.savetxt("out.csv", vpu_int, fmt="%i", delimiter=",")

    # small signals are always going to be ropey due to quantizing, so just check average error of top half
    top_half = utils.db(output_flt) > -50
    if np.any(top_half):
        error_flt = np.abs(utils.db(output_int[top_half])-utils.db(output_flt[top_half]))
        mean_error_flt = utils.db(np.nanmean(utils.db2gain(error_flt)))
        assert mean_error_flt < 0.055
        error_vpu = np.abs(utils.db(output_int[top_half])-utils.db(output_vpu[top_half]))
        mean_error_vpu = utils.db(np.nanmean(utils.db2gain(error_vpu)))
        assert mean_error_vpu < 0.05


def test_peq_saturation(fs):
    # a list of some sensible filters, use them in  random order
    filter_spec = [['notch', fs*0.05, 1],
                   ['notch', fs*0.10, 1],
                   ['notch', fs*0.15, 1],
                   ['notch', fs*0.20, 1],
                   ['notch', fs*0.25, 1],
                   ['notch', fs*0.30, 1],
                   ['notch', fs*0.35, 1],
                   ['lowshelf', fs*1000/48000, 1, 3]]

    peq = cbq.parametric_eq_8band(fs, 1, filter_spec, Q_sig=31)
    saturation_test(peq, fs)


if __name__ == "__main__":
    test_peq_saturation(48000)
