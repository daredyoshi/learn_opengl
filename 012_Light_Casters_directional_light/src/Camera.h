#pragma once

#include "glm/ext/matrix_clip_space.hpp"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/trigonometric.hpp"
#include "glm/fwd.hpp"
#include "glm/gtc/matrix_access.hpp"
#include "glm/ext/matrix_transform.hpp"
#define GLM_SWIZZLE

const float DEFAULT_YAW                 {   -90.0f           };
const float DEFAULT_PITCH               {    0.0f            };
const float DEFAULT_ROLL                 {    0.0f            };
const float DEFAULT_MOVE_SPEED          {    4.5f            };
const float DEFAULT_DIR_SPEED           {    70.0f           };
const float DEFAULT_MOUSE_SENSITIVITY   {    0.1f            };
const float DEFAULT_FOV                 {    45.0f           };
const glm::vec3 DEFAULT_POS             { 0.0f, 0.0f, 3.0f   };
const glm::vec3 DEFAULT_DIR             { 0.0f, 0.0f,-1.0f   };
const glm::vec3 DEFAULT_UP              { 0.0f, 1.0f, 0.0f   };

const glm::vec3 GLOBAL_UP               { 0.0f, 1.0f, 0.0f   };

class Camera {
private:
    // the position of our camera
    glm::vec3 m_pos;
    // the direction fo our camera
    // tecnically this is pointing in the reverse direction of what it is targeting
    glm::vec3 m_dir;
    // glm::vec3 camZ { glm::normalize(cameraPos - cameraTarget) };
    // get the right access by crossing up with the direction
    glm::vec3 m_up;
    double m_yaw ;
    double m_pitch;
    double m_roll;
    double m_moveSpeed;
    double m_dirSpeed;
    double m_mouseSensitivity;
    double m_fov ;



    // this is to check for if it's the starting cam
    bool m_firstMouse { true };
    double m_lastX{0};
    double m_lastY{0};

    double m_width { 800.0f };
    double m_height { 600.0f };
    double m_near { 0.1f };
    double m_far { 100.0f };

    void updateDirUp();

public:

    Camera( glm::vec3 pos=DEFAULT_POS, 
            glm::vec3 dir=DEFAULT_DIR, 
            glm::vec3 up=DEFAULT_UP, 
            double yaw=DEFAULT_YAW, 
            double pitch=DEFAULT_PITCH,
            double roll=DEFAULT_ROLL,
            double moveSpeed=DEFAULT_MOVE_SPEED, 
            double dirSpeed=DEFAULT_DIR_SPEED,
            double mouseSensitivity=DEFAULT_MOUSE_SENSITIVITY,
            double fov=DEFAULT_FOV) 
        : 
            m_pos{pos}, 
            m_dir{dir}, 
            m_up{up}, 
            m_yaw{yaw}, 
            m_pitch{pitch}, 
            m_roll{roll},
            m_moveSpeed{moveSpeed}, 
            m_dirSpeed{dirSpeed},
            m_mouseSensitivity{mouseSensitivity}, 
            m_fov{fov} 
        {} 
            
    enum class CameraDirection {
        FORWARD,
        BACKWARD,
        LEFT,
        RIGHT,
        UP,
        DOWN,
        LEFT_ROLL,
        RIGHT_ROLL
    };

    glm::mat4 getProjectionMatrix() const {
        return glm::perspective(glm::radians(m_fov), m_width / m_height, m_near, m_far);
    };
    
    glm::mat4 getViewMatrix() const {
        return glm::lookAt(m_pos, m_pos + m_dir, m_up);
    }

    glm::vec3 getPos() const { return m_pos; }

    void changeFov(const float offset);
    void setSize(const double width, const double height) { m_width = width; m_height = height; }
    // set the direction based on an xposition and a yposition 
    // (usually from mouse input)
    void setDirection(const double xpos, const double ypos);
    void incrementDirection(const CameraDirection direction, const float deltaTime);
    void incrementPosition(const CameraDirection direction, const float deltaTime, const bool useGlobalUp=false);
};
