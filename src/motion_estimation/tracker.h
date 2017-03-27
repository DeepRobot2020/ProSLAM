#pragma once
#include "aligners/stereouv_aligner.h"
#include "types/world_map.h"
#include "triangulation/stereo_triangulator.h"

namespace proslam {

  //ds this class
  class Tracker {
  public: EIGEN_MAKE_ALIGNED_OPERATOR_NEW

  //ds object handling
  public:

    Tracker(const Camera* camera_left_, const Camera* camera_right_);
    ~Tracker();
    Tracker() = delete;

  //ds functionality
  public:

    const TransformMatrix3D compute(WorldMap* context_,
                                    const cv::Mat& intensity_image_left_,
                                    const cv::Mat& intensity_image_right_,
                                    const TransformMatrix3D& initial_guess_transform_previous_to_current_ = TransformMatrix3D::Identity());

  //ds getters/setters
  public:

    BaseAligner6_4* aligner() {return _pose_optimizer;}
    StereoTriangulator* preprocessor() {return _framepoint_generator;}
    const Count totalNumberOfTrackedPoints() const {return _total_number_of_tracked_points;}
    const Count totalNumberOfLandmarksClose() const {return _total_number_of_landmarks_close;}
    const Count totalNumberOfLandmarksFar() const {return _total_number_of_landmarks_far;}
    void setPixelDistanceTrackingMaximum(const int32_t& pixel_distance_tracking_) {_pixel_distance_tracking_threshold_maximum = pixel_distance_tracking_;}
    void setPixelDistanceTrackingMinimum(const int32_t& pixel_distance_tracking_) {_pixel_distance_tracking_threshold_minimum = pixel_distance_tracking_;}

  //ds helpers
  protected:

    void _trackFeatures(Frame* previous_frame_, Frame* current_frame_);
    void _extractFeatures(Frame* frame_);
    void _getImageCoordinates(std::vector<ImageCoordinates>& predictions_left,
                              Frame* previous_frame_,
                              const Frame* current_frame_) const;
    void _updateLandmarks(WorldMap* context_);
    void _recoverPoints(WorldMap* context_);

  protected:

    //ds tracker status
    Frame::Status _status          = Frame::Localizing;
    Frame::Status _status_previous = Frame::Localizing;

    //ds running variables and buffered values
    Count _minimum_number_of_landmarks_to_track = 5;
    Count _number_of_tracked_landmarks_far      = 0;
    Count _number_of_tracked_landmarks_close    = 0;
    Count _number_of_potential_points           = 0;
    Count _number_of_tracked_points             = 0;
    const Camera* _camera_left;
    const Camera* _camera_right;
    const int32_t _camera_rows;
    const int32_t _camera_cols;

    //ds framepoint retrieval
    StereoTriangulator* _framepoint_generator;

    //ds framepoint tracking configuration
    int32_t _pixel_distance_tracking_threshold           = 0;   //ds current pixel distance threshold for framepoint tracking - lower means higher precision
    int32_t _pixel_distance_tracking_threshold_maximum   = 7*7; //ds upper limit: 7x7 pixels
    int32_t _pixel_distance_tracking_threshold_minimum   = 4*4; //ds lower limit: 4x4 pixels
    const int32_t _range_point_tracking        = 2;       //ds pixel search range width for point vicinity tracking
    const int32_t _maximum_flow_pixels_squared = 150*150; //ds maximum allowed pixel distance between image coordinates prediction and actual detection

    //ds pose solving
    StereoUVAligner* _pose_optimizer;

    //ds framepoint track recovery
    Count _number_of_lost_points           = 0;
    Count _number_of_lost_points_recovered = 0;
    std::vector<FramePoint*> _lost_points;

    //ds buffers
    std::vector<ImageCoordinates> _projected_image_coordinates_left;

    //ds informative only
    CREATE_CHRONOMETER(tracking)
    CREATE_CHRONOMETER(track_creation)
    CREATE_CHRONOMETER(pose_optimization)
    CREATE_CHRONOMETER(landmark_optimization)
    CREATE_CHRONOMETER(point_recovery)
    Count _total_number_of_tracked_points  = 0;
    Count _total_number_of_landmarks_close = 0;
    Count _total_number_of_landmarks_far   = 0;
    public: const double getTimeConsumptionSeconds_feature_detection() const {return _framepoint_generator->getTimeConsumptionSeconds_feature_detection();}
    public: const double getTimeConsumptionSeconds_keypoint_pruning() const {return _framepoint_generator->getTimeConsumptionSeconds_keypoint_pruning();}
    public: const double getTimeConsumptionSeconds_descriptor_extraction() const {return _framepoint_generator->getTimeConsumptionSeconds_descriptor_extraction();}
    public: const double getTimeConsumptionSeconds_point_triangulation() const {return _framepoint_generator->getTimeConsumptionSeconds_point_triangulation();}
  };
}