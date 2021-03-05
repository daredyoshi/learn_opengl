#include "Camera.h"
#include "glm/ext/quaternion_geometric.hpp"
#include "glm/fwd.hpp"
#include "glm/geometric.hpp"
#include "glm/gtx/transform.hpp"

void Camera::incrementDirection(CameraDirection direction, const float deltaTime){    
    // since there is no direction input, reset the mouse
    m_firstMouse = true;

    double velocity = m_dirSpeed * deltaTime;
    switch ( direction ){
        case CameraDirection::UP:
            m_pitch += velocity;
            break;
        case CameraDirection::DOWN:
            m_pitch -= velocity;
            break;
        case CameraDirection::LEFT:
            m_yaw -= velocity;
            break;
        case CameraDirection::RIGHT:
            m_yaw += velocity;
            break;
        case CameraDirection::LEFT_ROLL:
            m_roll += velocity;
            break;
        case CameraDirection::RIGHT_ROLL:
            m_roll -= velocity;
        default:
            break;
    }
    updateDirUp();
}

void Camera::incrementPosition(const CameraDirection direction, const float deltaTime, const bool useGlobalUp){
    float velocity = m_moveSpeed * deltaTime;
    glm::vec3 upDir = useGlobalUp ? GLOBAL_UP : m_up; 
    upDir = m_up;
    glm::vec3 forwardDir = m_dir;
    forwardDir[1] = 0;
    forwardDir = glm::normalize(forwardDir);
    glm::vec3 leftDir{ glm::normalize(glm::cross(forwardDir, m_up)) };
    switch ( direction ){
        case CameraDirection::UP:
            m_pos += upDir * velocity;
            return;
        case CameraDirection::DOWN:
            m_pos -= upDir * velocity;
            return;
        case CameraDirection::FORWARD:
            m_pos += forwardDir * velocity; 
            return; 
        case CameraDirection::BACKWARD:
            m_pos -= forwardDir * velocity;
            return;
        case CameraDirection::LEFT:
            m_pos -= leftDir * velocity; 
            return;
        case CameraDirection::RIGHT:
            m_pos += leftDir * velocity;
            return;
        default:
            return; 
    }
}

void Camera::setDirection(const double xpos, const double ypos){

    // first run we don't want to overwrite to keep initial cam
    if (m_firstMouse){
        m_lastX = xpos;
        m_lastY = ypos;
        m_firstMouse = false;
    }


    float xOffset = xpos - m_lastX;    
    float yOffset = ypos - m_lastY;
    m_lastX = xpos;
    m_lastY = ypos;

    const float sensitivity = 0.1f;
    xOffset *= sensitivity;
    yOffset *= sensitivity;
    
    m_yaw += xOffset;
    m_pitch -= yOffset;

    // add constraints to avoid flipping
    if (m_pitch > 89.0f)
        m_pitch = 89.0f;
    if(m_pitch < -89.0f)
        m_pitch = -89.0f;

    // calculate the direction of the camera
    updateDirUp();
}

void Camera::updateDirUp(){
    glm::vec3 direction;
    direction.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
    direction.y = sin(glm::radians(m_pitch));
    direction.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
    // glm::vec3 up;
    // up.y = cos(glm::radians(m_roll));
    // up.x = sin(glm::radians(m_roll));

    m_dir = glm::normalize(direction); 
    // glm::vec3 right{ glm::normalize(glm::cross(direction, up)) };
    glm::mat4 roll_mat = glm::rotate(glm::mat4(1.0f), float(glm::radians(m_roll)), m_dir);
    m_up = glm::mat3(roll_mat) * GLOBAL_UP;
}

// changes the fov by the offset
void Camera::changeFov(const float offset){
    m_fov -= offset;
    if (m_fov < 1.0f)
        m_fov = 1.0f;
    if (m_fov > 80.0f)
        m_fov = 80.0f;
}
