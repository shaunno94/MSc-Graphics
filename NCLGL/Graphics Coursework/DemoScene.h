#include <nclgl\Scene.h>

class DemoScene : public Scene
{
public:
	DemoScene();
	virtual ~DemoScene();

	virtual void UpdateScene(float msec) override;
	virtual void DrawScene(bool shadowPass) override;
	virtual void LateDraw() override;

protected:

private:

	Light* pb_light;
	Vector3 heightMap_center;

	unsigned int envLight_index;
	unsigned int skybox_index;
};