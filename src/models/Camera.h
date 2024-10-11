#include <vector>
class Camera
{
private:
    std::vector<int> u;
    std::vector<int> v;
    // in camera space
    std::vector<int> position; // e
    std::vector<int> w;
public:
    Camera(std::vector<int> u, std::vector<int> v, std::vector<int> position);
    std::vector<int> getPosition();
};

