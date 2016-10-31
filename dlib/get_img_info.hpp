#ifndef OCV_DLIB_GET_IMG_INFO_HPP
#define OCV_DLIB_GET_IMG_INFO_HPP

#include <dlib/geometry.h>

#include <string>
#include <vector>

/*!
 *  \addtogroup ocv
 *  @{
 */
namespace ocv{

/*!
 *  \addtogroup odlib
 *  @{
 */
namespace odlib{

/**
 * @brief store the image name and associate bounding rect of the
 * xml file generated by imglab(tool of dlib) to img_name and location
 * @param img_name name of the images
 * @param roi roi of the images
 * @param file_name full path of the xml file generated by imglab
 */
void get_imglab_xml_info(std::vector<std::string> &img_name,
                         std::vector<std::vector<dlib::rectangle>> &roi,
                         std::string const &file_name);


} /*! @} End of Doxygen Groups*/

} /*! @} End of Doxygen Groups*/

#endif // OCV_DLIB_GET_IMG_NAME_AND_POSITION_HPP
