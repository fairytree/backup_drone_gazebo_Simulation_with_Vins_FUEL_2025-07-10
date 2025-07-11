#ifndef _PLANNING_VISUALIZATION_H_
#define _PLANNING_VISUALIZATION_H_

#include <Eigen/Eigen>
#include <algorithm>
#include <bspline/non_uniform_bspline.h>
#include <iostream>
#include <path_searching/topo_prm.h>
#include <plan_env/obj_predictor.h>
#include <poly_traj/polynomial_traj.h>
#include <vector>
#include <active_perception/traj_visibility.h>
#include "rclcpp/rclcpp.hpp"
#include "visualization_msgs/msg/marker.hpp"

using std::vector;
namespace fast_planner
{
  class PlanningVisualization
  {
  private:
    enum TRAJECTORY_PLANNING_ID
    {
      GOAL = 1,
      PATH = 200,
      BSPLINE = 300,
      BSPLINE_CTRL_PT = 400,
      POLY_TRAJ = 500
    };

    enum TOPOLOGICAL_PATH_PLANNING_ID
    {
      GRAPH_NODE = 1,
      GRAPH_EDGE = 100,
      RAW_PATH = 200,
      FILTERED_PATH = 300,
      SELECT_PATH = 400
    };

    /* data */
    /* visib_pub is seperated from previous ones for different info */
    std::shared_ptr<rclcpp::Node> node_;
    rclcpp::Publisher<visualization_msgs::msg::Marker>::SharedPtr traj_pub_;      // 0
    rclcpp::Publisher<visualization_msgs::msg::Marker>::SharedPtr topo_pub_;      // 1
    rclcpp::Publisher<visualization_msgs::msg::Marker>::SharedPtr predict_pub_;   // 2
    rclcpp::Publisher<visualization_msgs::msg::Marker>::SharedPtr visib_pub_;     // 3, visibility constraints
    rclcpp::Publisher<visualization_msgs::msg::Marker>::SharedPtr frontier_pub_;  // 4, frontier searching
    rclcpp::Publisher<visualization_msgs::msg::Marker>::SharedPtr yaw_pub_;       // 5, yaw trajectory
    rclcpp::Publisher<visualization_msgs::msg::Marker>::SharedPtr viewpoint_pub_; // 6,  viewpoint planning
    vector<rclcpp::Publisher<visualization_msgs::msg::Marker>::SharedPtr> pubs_;  //

    int last_topo_path1_num_;
    int last_topo_path2_num_;
    int last_bspline_phase1_num_;
    int last_bspline_phase2_num_;
    int last_frontier_num_;

  public:
    PlanningVisualization(/* args */)
    {
    }
    ~PlanningVisualization()
    {
    }
    PlanningVisualization(const std::shared_ptr<rclcpp::Node> &node);

    // new interface
    void fillBasicInfo(visualization_msgs::msg::Marker &mk, const Eigen::Vector3d &scale,
                       const Eigen::Vector4d &color, const string &ns, const int &id, const int &shape);
    void fillGeometryInfo(visualization_msgs::msg::Marker &mk, const vector<Eigen::Vector3d> &list);
    void fillGeometryInfo(visualization_msgs::msg::Marker &mk, const vector<Eigen::Vector3d> &list1,
                          const vector<Eigen::Vector3d> &list2);

    void drawSpheres(const vector<Eigen::Vector3d> &list, const double &scale,
                     const Eigen::Vector4d &color, const string &ns, const int &id, const int &pub_id);
    void drawCubes(const vector<Eigen::Vector3d> &list, const double &scale, const Eigen::Vector4d &color,
                   const string &ns, const int &id, const int &pub_id);
    void drawLines(const vector<Eigen::Vector3d> &list1, const vector<Eigen::Vector3d> &list2,
                   const double &scale, const Eigen::Vector4d &color, const string &ns, const int &id,
                   const int &pub_id);
    void drawLines(const vector<Eigen::Vector3d> &list, const double &scale, const Eigen::Vector4d &color,
                   const string &ns, const int &id, const int &pub_id);
    void drawBox(const Eigen::Vector3d &center, const Eigen::Vector3d &scale, const Eigen::Vector4d &color,
                 const string &ns, const int &id, const int &pub_id);

    // Deprecated
    // draw basic shapes
    void displaySphereList(const vector<Eigen::Vector3d> &list, double resolution,
                           const Eigen::Vector4d &color, int id, int pub_id = 0);
    void displayCubeList(const vector<Eigen::Vector3d> &list, double resolution,
                         const Eigen::Vector4d &color, int id, int pub_id = 0);
    void displayLineList(const vector<Eigen::Vector3d> &list1, const vector<Eigen::Vector3d> &list2,
                         double line_width, const Eigen::Vector4d &color, int id, int pub_id = 0);
    // draw a piece-wise straight line path
    void drawGeometricPath(const vector<Eigen::Vector3d> &path, double resolution,
                           const Eigen::Vector4d &color, int id = 0);
    // draw a polynomial trajectory
    void drawPolynomialTraj(PolynomialTraj poly_traj, double resolution, const Eigen::Vector4d &color,
                            int id = 0);
    // draw a bspline trajectory
    void drawBspline(NonUniformBspline &bspline, double size, const Eigen::Vector4d &color,
                     bool show_ctrl_pts = false, double size2 = 0.1,
                     const Eigen::Vector4d &color2 = Eigen::Vector4d(1, 1, 0, 1), int id = 0);
    // draw a set of bspline trajectories generated in different phases
    void drawBsplinesPhase1(vector<NonUniformBspline> &bsplines, double size);
    void drawBsplinesPhase2(vector<NonUniformBspline> &bsplines, double size);
    // draw topological graph and paths
    void drawTopoGraph(list<GraphNode::Ptr> &graph, double point_size, double line_width,
                       const Eigen::Vector4d &color1, const Eigen::Vector4d &color2,
                       const Eigen::Vector4d &color3, int id = 0);
    void drawTopoPathsPhase1(vector<vector<Eigen::Vector3d>> &paths, double line_width);
    void drawTopoPathsPhase2(vector<vector<Eigen::Vector3d>> &paths, double line_width);

    void drawGoal(Eigen::Vector3d goal, double resolution, const Eigen::Vector4d &color, int id = 0);
    void drawPrediction(ObjPrediction pred, double resolution, const Eigen::Vector4d &color, int id = 0);

    Eigen::Vector4d getColor(const double &h, double alpha = 1.0);

    typedef std::shared_ptr<PlanningVisualization> Ptr;

    // SECTION developing
    void drawVisibConstraint(const Eigen::MatrixXd &ctrl_pts, const vector<Eigen::Vector3d> &block_pts);
    void drawVisibConstraint(const Eigen::MatrixXd &pts, const vector<VisiblePair> &pairs);
    void drawViewConstraint(const ViewConstraint &vc);
    void drawFrontier(const vector<vector<Eigen::Vector3d>> &frontiers);
    void drawYawTraj(NonUniformBspline &pos, NonUniformBspline &yaw, const double &dt);
    void drawYawPath(NonUniformBspline &pos, const vector<double> &yaw, const double &dt);
  };
} // namespace fast_planner
#endif