#include "playground.hpp"

class GameObject;

class Component : public spk::ActivableObject{
protected:
    std::shared_ptr<GameObject> _owner;

    virtual bool _onUpdate() = 0;
    virtual void _onRender() = 0;

public:
    Component(std::shared_ptr<GameObject> p_owner);

    std::shared_ptr<GameObject> owner() const;

    bool update();
    void render();
};

class Transform : public Component
{
protected:
    virtual bool _onUpdate() override;
    virtual void _onRender() override;

    spk::Vector3 calculateRotationFromVectors(const spk::Vector3& right, const spk::Vector3& up, const spk::Vector3& forward);

public:
    spk::Vector3 position;
    spk::Vector3 size;
    spk::Vector3 rotation;

    Transform(std::shared_ptr<GameObject> p_owner);

    void lookAt(const spk::Vector3& target, const spk::Vector3& up = spk::Vector3(0, 1, 0));
};

class Camera : public Component
{
public:
    enum class CameraType {
        Perspective,
        Orthographic
    };

private:
    static inline Camera* _mainCamera = nullptr;
    CameraType _type;
    spk::Matrix4x4 _projectionMatrix;

    // --- Perspective camera attributes
    float _fov;
    float _aspectRatio;
    float _nearPlane;
    float _farPlane;

    // --- Orthographic camera attributes
    spk::Vector2 _size;

    void updateProjectionMatrix();

protected:
    virtual bool _onUpdate() override;
    virtual void _onRender() override;

public:
    Camera(std::shared_ptr<GameObject> p_owner, CameraType p_type = CameraType::Perspective);

    void setAsMainCamera();
    void setType(CameraType p_type);
    void setPerspectiveParameters(float p_fov, float p_aspectRatio, float p_nearPlane, float p_farPlane);
    void setOrthographicParameters(const spk::Vector2& p_size, float p_nearPlane, float p_farPlane);
    spk::Matrix4x4 getMVPMatrix(const spk::Matrix4x4& modelMatrix) const;
};

class GameObject : public spk::ActivableObject, public spk::InherenceObject<GameObject>
{
private:
    std::wstring _name;
    std::vector<std::shared_ptr<Component>> _components;
    std::shared_ptr<Transform> _transform;

public:
    GameObject(const std::wstring& p_name);

    template <typename TComponentType, typename... Args>
    std::shared_ptr<TComponentType> addComponent(Args&&... p_args)
    {
        std::shared_ptr<TComponentType> result = std::make_shared<TComponentType>(std::forward<Args>(p_args)...);

        return (result);
    }

    template <typename TComponentType>
    std::shared_ptr<TComponentType> getComponent()
    {
        for (auto& component : _components)
        {
            std::shared_ptr<TComponentType> castedComponent = std::dynamic_pointer_cast<TComponentType>(component);
            if (castedComponent != nullptr)
            {
                return castedComponent;
            }
        }
        return nullptr;
    }

    std::shared_ptr<GameObject> addChildrenGameObject(const std::wstring& p_childrenName);

    const std::wstring& name() const;
    std::shared_ptr<Transform> transform();
    std::shared_ptr<const Transform> transform() const;

    void render();
    bool update();
};

class GameEngine
{
private:
    std::vector<std::shared_ptr<GameObject>> _ownedGameObjects;

public:
    GameEngine();

    bool update();
    void render();
    
    std::shared_ptr<GameObject> subscribe(std::shared_ptr<GameObject> p_objectToAdd);
    void unsubscribe(std::shared_ptr<GameObject> p_objectToRemove);
    void unsubscribe(const std::wstring& p_objectName);
};

Component::Component(std::shared_ptr<GameObject> p_owner) :
    _owner(p_owner)
{

}

std::shared_ptr<GameObject> Component::owner() const
{
    return (_owner);
}

bool Component::update()
{
    if (isActive() == true)
    {
        return (_onUpdate());
    }
    return (false);
}

void Component::render()
{
    if (isActive() == true)
    {
        _onRender();
    }
}

Transform::Transform(std::shared_ptr<GameObject> p_owner)
        : Component(p_owner), position(0, 0, 0), size(1, 1, 1), rotation(0, 0, 0) {}

bool Transform::_onUpdate()
{
    return (false);
}

void Transform::_onRender()
{

}

spk::Vector3 Transform::calculateRotationFromVectors(const spk::Vector3& right, const spk::Vector3& up, const spk::Vector3& forward)
{
    // /!\ Right hand axis /!\ 
    return(spk::Vector3(
        std::atan2(forward.x, forward.z),
        std::atan2(-forward.y, std::sqrt(forward.x * forward.x + forward.z * forward.z)),
        std::atan2(right.y, up.y)
    ));
}

void Transform::lookAt(const spk::Vector3& target, const spk::Vector3& up)
{
    spk::Vector3 forward = (target - position).normalize();
    spk::Vector3 right = up.cross(forward).normalize();
    spk::Vector3 newUp = forward.cross(right);

    rotation = calculateRotationFromVectors(right, newUp, forward);
}

void Camera::updateProjectionMatrix()
{
    if (_type == CameraType::Perspective)
    {
        _projectionMatrix = spk::Matrix4x4::perspective(
            spk::degreeToRadian(_fov),
            _aspectRatio,
            _nearPlane,
            _farPlane
        );
    }
    else
    {
        float halfWidth = _size.x / 2.0f;
        float halfHeight = _size.y / 2.0f;
        _projectionMatrix = spk::Matrix4x4::ortho(
            -halfWidth, halfWidth,
            -halfHeight, halfHeight,
            _nearPlane, _farPlane
        );
    }
}

bool Camera::_onUpdate()
{
    return true;
}

void Camera::_onRender()
{
    
}

Camera::Camera(std::shared_ptr<GameObject> p_owner, CameraType p_type) :
    Component(p_owner),
    _type(p_type)
{
    setPerspectiveParameters(45.0f, 1.33f, 0.1f, 100.0f);
    setOrthographicParameters(spk::Vector2(20, 20), 0.1f, 100.0f);
    updateProjectionMatrix();
}

void Camera::setAsMainCamera()
{
    _mainCamera = this;
}

void Camera::setType(CameraType p_type) {
    _type = p_type;
    updateProjectionMatrix();
}

void Camera::setPerspectiveParameters(float p_fov, float p_aspectRatio, float p_nearPlane, float p_farPlane)
{
    _fov = p_fov;
    _aspectRatio = p_aspectRatio;
    _nearPlane = p_nearPlane;
    _farPlane = p_farPlane;
    if (_type == CameraType::Perspective) {
        updateProjectionMatrix();
    }
}
void Camera::setOrthographicParameters(const spk::Vector2& p_size, float p_nearPlane, float p_farPlane)
{
    _size = p_size;
    _nearPlane = p_nearPlane;
    _farPlane = p_farPlane;
    if (_type == CameraType::Orthographic) {
        updateProjectionMatrix();
    }
}

spk::Matrix4x4 Camera::getMVPMatrix(const spk::Matrix4x4& modelMatrix) const
{
    auto viewMatrix = spk::Matrix4x4::lookAt(owner()->transform()->position, owner()->transform()->position + owner()->transform()->rotation, spk::Vector3(0, 1, 0));
    return (_projectionMatrix * viewMatrix * modelMatrix);
}

GameObject::GameObject(const std::wstring& p_name)
{
    _transform = addComponent<Transform>(std::shared_ptr<GameObject>(this, [](GameObject*){}));
}

std::shared_ptr<GameObject> GameObject::addChildrenGameObject(const std::wstring& p_childrenName)
{
    std::shared_ptr<GameObject> result = std::make_shared<GameObject>(p_childrenName);

    result->setParent(this);

    return (result);
}

const std::wstring& GameObject::name() const
{
    return (_name);
}

std::shared_ptr<Transform> GameObject::transform()
{
    return (_transform);
}

std::shared_ptr<const Transform> GameObject::transform() const
{
    return (_transform);
}

void GameObject::render()
{
    if (isActive() == false)
        return ;

    for (size_t i = 0; i < _components.size(); i++)
    {
        _components[i]->render();
    }

    for (size_t i = 0; i < childrens().size(); i++)
    {
        childrens()[i]->render();
    }
}
bool GameObject::update()
{
    if (isActive() == false)
        return (false);

    for (size_t i = 0; i < childrens().size(); i++)
    {
        if (childrens()[i]->update() == true)
            return (true);
    }

    for (size_t i = 0; i < _components.size(); i++)
    {
        if (_components[i]->update() == true)
            return (true);
    }
    return (false);
}

GameEngine::GameEngine()
{

}

bool GameEngine::update()
{
    for (size_t i = 0; i < _ownedGameObjects.size(); i++)
    {
        if (_ownedGameObjects[i]->isActive() == true && _ownedGameObjects[i]->update() == true)
            return (true);
    }
    return (false);
}

void GameEngine::render()
{
    for (size_t i = 0; i < _ownedGameObjects.size(); i++)
    {
        if (_ownedGameObjects[i]->isActive() == true)
            _ownedGameObjects[i]->render();
    }
}

std::shared_ptr<GameObject> GameEngine::subscribe(std::shared_ptr<GameObject> p_objectToAdd)
{
    _ownedGameObjects.push_back(p_objectToAdd);

    return (p_objectToAdd);
}

void GameEngine::unsubscribe(std::shared_ptr<GameObject> p_objectToRemove)
{
    for (auto it = _ownedGameObjects.begin(); it != _ownedGameObjects.end(); ++it)
    {
        if (*it == p_objectToRemove)
        {
            _ownedGameObjects.erase(it);
            return;
        }
    }
}

void GameEngine::unsubscribe(const std::wstring& p_objectName)
{
    for (auto it = _ownedGameObjects.begin(); it != _ownedGameObjects.end(); ++it)
    {
        if ((*it)->name() == p_objectName)
        {
            _ownedGameObjects.erase(it);
            return;
        }
    }
}

int main()
{
    spk::Application app(L"Playground", 800);
    spk::Keyboard::instance()->setLayout(spk::Keyboard::Layout::Azerty);
  
    return (app.run());
}
