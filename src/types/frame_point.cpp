#include "frame_point.h"

namespace proslam {

  //ds inner instance count - incremented upon constructor call (also unsuccessful calls)
  Count FramePoint::_instances = 0;

  //ds construct a new framepoint, owned by the provided Frame
  FramePoint::FramePoint(const cv::KeyPoint& keypoint_left_,
                         const cv::Mat& descriptor_left_,
                         const cv::KeyPoint& keypoint_right_,
                         const cv::Mat& descriptor_right_,
                         Frame* frame_): _identifier(_instances),
                                         _keypoint_left(keypoint_left_),
                                         _keypoint_right(keypoint_right_),
                                         _descriptor_left(descriptor_left_),
                                         _descriptor_right(descriptor_right_),
                                         _image_coordinates_left(PointCoordinates(keypoint_left_.pt.x, keypoint_left_.pt.y, 1)),
                                         _image_coordinates_right(PointCoordinates(keypoint_right_.pt.x, keypoint_right_.pt.y, 1)) {
    ++_instances;
    setFrame(frame_);
  }

  //ds connect current FramePoint to a previous one
  void FramePoint::setPrevious(FramePoint* previous_) {
    _previous = previous_;
    setLandmark(previous_->landmark());
    setTrackLength(previous_->trackLength()+1);
    setOrigin(previous_->origin());
  }
}