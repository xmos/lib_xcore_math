
BFP API Quick Reference
=======================

The tables below list the functions of the block floating-point API. The "EW" column indicates 
whether the operation acts element-wise.

The "Signature" column is intended as a hint which quickly conveys the kind of the conceptual inputs 
to and outputs from the operation.  The signatures are only intended to convey how many (conceptual) 
inputs and outputs there are, and their dimensionality.  

The functions themselves will typically take more arguments than these signatures indicate.  Check 
the function's full documentation to get more detailed information.

The following symbols are used in the signatures:

.. table::
    :widths: 40 60
    :class: longtable

    +--------------------------------------+---------------------------------------------+
    |  Symbol                              | Description                                 |
    +======================================+=============================================+
    | :math:`\mathbb{S}`                   | A scalar input or output value.             |
    +--------------------------------------+---------------------------------------------+
    | :math:`\mathbb{V}`                   | A vector-valued input or output.            |
    +--------------------------------------+---------------------------------------------+
    | :math:`\mathbb{M}`                   | A matrix-valued input or output.            |
    +--------------------------------------+---------------------------------------------+
    | :math:`\varnothing`                  | Placeholder indicating no input or output.  |
    +--------------------------------------+---------------------------------------------+

For example, the operation signature :math:`(\mathbb{V \times V \times S}) \to \mathbb{V}` indicates
the operation takes two vector inputs and a scalar input, and the output is a vector.


* `32-Bit BFP Ops <bfp32_api_>`_
* `16-Bit BFP Ops <bfp16_api_>`_
* `Complex 32-Bit BFP Ops <bfp32_complex_api_>`_
* `Complex 16-Bit BFP Ops <bfp16_complex_api_>`_

.. raw:: latex

  \newpage

32-Bit BFP API Quick Reference
------------------------------

.. _bfp32_api:

.. csv-table:: 32-Bit BFP API - Quick Reference
    :file: csv/32bit_bfp_quickref.csv
    :widths: 42, 5, 20, 33
    :header-rows: 1
    :class: longtable

.. raw:: latex

  \newpage

16-Bit BFP API Quick Reference
------------------------------

.. _bfp16_api:

.. csv-table:: 16-Bit BFP API - Quick Reference
    :file: csv/16bit_bfp_quickref.csv
    :widths: 42, 5, 20, 33
    :header-rows: 1
    :class: longtable

.. raw:: latex

  \newpage

Complex 32-bit BFP API Quick Reference
--------------------------------------

.. _bfp32_complex_api:

.. csv-table:: Complex 32-Bit BFP API - Quick Reference
    :file: csv/complex_32bit_bfp_quickref.csv
    :widths: 42, 5, 20, 33
    :header-rows: 1
    :class: longtable


.. raw:: latex

  \newpage

Complex 16-bit BFP API Quick Reference
--------------------------------------

.. _bfp16_complex_api:

.. csv-table:: Complex 16-Bit BFP API - Quick Reference
    :file: csv/complex_16bit_bfp_quickref.csv
    :widths: 42, 5, 20, 33
    :header-rows: 1
    :class: longtable


.. raw:: latex

  \newpage
