// Copyright (C) 2006  Davis E. King (davis@dlib.net)
// License: Boost Software License   See LICENSE.txt for the full license.
#undef DLIB_THRESHOLDINg_ABSTRACT_
#ifdef DLIB_THRESHOLDINg_ABSTRACT_ 

#include "../pixel.h"

namespace dlib
{

// ----------------------------------------------------------------------------------------

    const unsigned char on_pixel = 255;
    const unsigned char off_pixel = 0;

// ----------------------------------------------------------------------------------------

    template <
        typename in_image_type,
        typename out_image_type
        >
    void threshold_image (
        const in_image_type& in_img,
        out_image_type& out_img,
        unsigned long thresh
    );
    /*!
        requires
            - in_image_type == is an implementation of array2d/array2d_kernel_abstract.h
            - out_image_type == is an implementation of array2d/array2d_kernel_abstract.h
            - pixel_traits<typename out_image_type::type>::grayscale == true  
            - pixel_traits<typename in_image_type::type>::has_alpha == false
            - pixel_traits<typename out_image_type::type>::has_alpha == false 
        ensures
            - #out_img == the thresholded version of in_img (in_img is converted to a grayscale
              intensity image if it is color).  Pixels in in_img with grayscale values >= thresh 
              have an output value of on_pixel and all others have a value of off_pixel.
            - #out_img.nc() == in_img.nc()
            - #out_img.nr() == in_img.nr()
    !*/

// ----------------------------------------------------------------------------------------

    template <
        typename in_image_type,
        typename out_image_type
        >
    void auto_threshold_image (
        const in_image_type& in_img,
        out_image_type& out_img
    );
    /*!
        requires
            - in_image_type == is an implementation of array2d/array2d_kernel_abstract.h
            - out_image_type == is an implementation of array2d/array2d_kernel_abstract.h
            - pixel_traits<typename out_image_type::type>::grayscale == true  
            - pixel_traits<typename in_image_type::type>::has_alpha == false
            - pixel_traits<typename out_image_type::type>::has_alpha == false 
            - pixel_traits<typename in_image_type::type>::is_unsigned == true 
            - pixel_traits<typename out_image_type::type>::is_unsigned == true 
        ensures
            - #out_img == the thresholded version of in_img (in_img is converted to a grayscale
              intensity image if it is color).  Pixels in in_img with grayscale values >= thresh 
              have an output value of on_pixel and all others have a value of off_pixel.
            - The thresh value used is determined by performing a k-means clustering
              on the input image histogram with a k of 2.  The point between the two
              means found is used as the thresh value.
            - #out_img.nc() == in_img.nc()
            - #out_img.nr() == in_img.nr()
    !*/

// ----------------------------------------------------------------------------------------

    template <
        typename in_image_type,
        typename out_image_type
        >
    void hysteresis_threshold (
        const in_image_type& in_img,
        const out_image_type& out_img,
        unsigned long lower_thresh,
        unsigned long upper_thresh
    );
    /*!
        requires
            - in_image_type == is an implementation of array2d/array2d_kernel_abstract.h
            - out_image_type == is an implementation of array2d/array2d_kernel_abstract.h
            - pixel_traits<typename out_image_type::type>::grayscale == true  
            - pixel_traits<typename in_image_type::type>::has_alpha == false
            - pixel_traits<typename out_image_type::type>::has_alpha == false 
            - lower_thresh <= upper_thresh
        ensures
            - #out_img == the hysteresis thresholded version of in_img (in_img is converted to a 
              grayscale intensity image if it is color). Pixels in in_img with grayscale 
              values >= upper_thresh have an output value of on_pixel and all others have a 
              value of off_pixel unless they are >= lower_thresh and are adjacent to a pixel
              with a value >= upper_thresh in which case they have a value of on_pixel.
            - #out_img.nc() == in_img.nc()
            - #out_img.nr() == in_img.nr()
    !*/

// ----------------------------------------------------------------------------------------

}

#endif // DLIB_THRESHOLDINg_ABSTRACT_ 


