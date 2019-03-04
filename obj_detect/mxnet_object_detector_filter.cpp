#include "mxnet_object_detector_filter.hpp"

namespace ocv{

namespace obj_det{

mxnet_object_detector_filter::
mxnet_object_detector_filter(const std::vector<coco_item_type> &items_to_detect,
                             cv::Size const &obj_detector_process_size,
                             float min_confidence) :
    min_confidence_(min_confidence),
    obj_detector_process_size_(obj_detector_process_size)
{
    items_to_detect_.resize(80, false);
    set_items_to_detect(items_to_detect);
}

std::vector<mxnet_object_detector_filter::result_type>
mxnet_object_detector_filter::filter(const std::vector<mxnet::cpp::NDArray> &input, cv::Size const &image_size) const
{
    using namespace mxnet::cpp;

    //1. predict_results get from the output of Executor(executor_->outputs)
    //2. Must Set Context as cpu because we need process data by cpu later
    auto labels = input[0].Copy(Context(kCPU, 0));
    auto scores = input[1].Copy(Context(kCPU, 0));
    auto bboxes = input[2].Copy(Context(kCPU, 0));

    //1. Should call wait because Forward api of Executor is async
    //2. scores and labels could treat as one dimension array
    //3. BBoxes can treat as 2 dimensions array
    bboxes.WaitToRead();
    scores.WaitToRead();
    labels.WaitToRead();

    std::vector<result_type> result;
    size_t const num = bboxes.GetShape()[1];
    for(size_t i = 0; i < num; ++i) {
        auto const score = scores.At(0, 0, i);
        auto const label = static_cast<size_t>(labels.At(0, 0, i));
        if(score >= min_confidence_ && label < items_to_detect_.size() && items_to_detect_[label]){
            result_type rtype;
            rtype.confidence_ = score;
            rtype.item_ = static_cast<coco_item_type>(label);
            rtype.roi_ = normalize_points(bboxes.At(0, i, 0), bboxes.At(0, i, 1),
                                          bboxes.At(0, i, 2), bboxes.At(0, i, 3), image_size);
            result.emplace_back(std::move(rtype));
        }
    }

    return result;
}

void mxnet_object_detector_filter::set_min_confidence(float input) noexcept
{
    min_confidence_ = input;
}

void mxnet_object_detector_filter::
set_items_to_detect(const std::vector<coco_item_type> &items_to_detect)
{
    std::fill(std::begin(items_to_detect_), std::end(items_to_detect_), false);
    for(size_t i = 0; i != items_to_detect.size(); ++i){
        items_to_detect_[static_cast<size_t>(items_to_detect[i])] = true;
    }
}

void mxnet_object_detector_filter::set_obj_detector_process_size(const cv::Size &obj_detector_process_size) noexcept
{
    obj_detector_process_size_ = obj_detector_process_size;
    if(obj_detector_process_size_.height == 0){
        obj_detector_process_size_.height = 224;
    }
    if(obj_detector_process_size_.width == 0){
        obj_detector_process_size_.width = 224;
    }
}

cv::Rect mxnet_object_detector_filter::clip_points(float x1, float y1, float x2, float y2,
                                                   const cv::Size &image_size) const noexcept
{
    return cv::Rect(cv::Point(static_cast<int>(std::max(x1, 0.0f)), static_cast<int>(std::max(y1, 0.0f))),
                    cv::Point(static_cast<int>(std::min(x2, image_size.width - 1.0f)),
                              static_cast<int>(std::min(y2, image_size.height - 1.0f))));
}

cv::Rect mxnet_object_detector_filter::
normalize_points(float x1, float y1, float x2, float y2, cv::Size const &image_size) const noexcept
{
    if(obj_detector_process_size_.height != image_size.height || obj_detector_process_size_.width != image_size.width){
        x1 = x1 / obj_detector_process_size_.width * image_size.width;
        y1 = y1 / obj_detector_process_size_.height * image_size.height;
        x2 = x2 / obj_detector_process_size_.width * image_size.width;
        y2 = y2 / obj_detector_process_size_.height * image_size.height;
        return clip_points(x1, y1, x2, y2, image_size);
    }else{
        return clip_points(x1, y1, x2, y2, image_size);
    }
}

}

}
