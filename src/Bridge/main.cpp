//
// Created by Alex on 27/10/2024.
//

#include <PhysX/System.h>
#include <PhysX/Constraint.h>

#include <orion/graphics.h>

#include <atomic>
#include <future>

template <class F, class... Args>
void SetInterval(std::atomic_bool& stop, size_t interval, F&& f, Args&&... args){
	std::thread([&]{
		while (!stop.load()){
			f(args...);
			std::this_thread::sleep_for(std::chrono::milliseconds(interval));
		}
	}).detach();
}

orion::Vector3f GlmToVec(const glm::vec3& v)
{
	return { v.x, v.y, v.z };
}

int main(int /* argc */, char** /* argv */)
{
	using namespace orion;

	std::atomic_bool stopPhysics = false;

	auto e = EventManager::create();
	auto w = Window::create("Bridge");
	w->set_monitor(nullptr, { 200, 200 }, { 1280, 720 }, 60);
	w->set_event_manager(e);

	e->subscribe<Input::KeyEvent>([&](const auto& e) {
		if (e.key == Input::Key::ESCAPE && e.pressed)
		{
			stopPhysics.store(true);
			e.window.lock()->close();
		}
	});

	auto& camera = w->get_view();
	camera.translate(orion::Vector3f(0.f, 0.f, 20.f));
	camera.look_at  (orion::Vector3f(0.f, 0.f, 0.f));

	Shape lan(Primitive::CIRCLE);
	lan.get_transform().set_scale({ 0.25f, 0.25f, 0.25f });
	Shape ran(Primitive::CIRCLE);
	ran.get_transform().set_scale({ 0.25f, 0.25f, 0.25f });
	Shape s1(Primitive::CIRCLE);
	s1.get_transform().set_scale({ 0.25f, 0.25f, 0.25f });
	Shape s2(Primitive::CIRCLE);
	s2.get_transform().set_scale({ 0.25f, 0.25f, 0.25f });
	Shape s3(Primitive::CIRCLE);
	s3.get_transform().set_scale({ 0.25f, 0.25f, 0.25f });
	Shape s4(Primitive::CIRCLE);
	s4.get_transform().set_scale({ 0.25f, 0.25f, 0.25f });
	Shape s5(Primitive::CIRCLE);
	s5.get_transform().set_scale({ 0.25f, 0.25f, 0.25f });
	Shape s6(Primitive::CIRCLE);
	s6.get_transform().set_scale({ 0.25f, 0.25f, 0.25f });
	Shape s7(Primitive::CIRCLE);
	s7.get_transform().set_scale({ 0.25f, 0.25f, 0.25f });

	float dt    = 0.01;
	auto  dt_ms = (size_t)(dt * 1000.f);

	// Rope property
	float k = 100.f;

	// Gravity property
	float g = 10.f;

	// Systems property
	float m = 1.f;
	float c = -(m * g) / (10.f * 10.f);

	StaticSystem lss(glm::vec3(-40, 5, 0));
	StaticSystem rss(glm::vec3( 40, 5, 0));

	VerletSystem vs1(dt, m, glm::vec3(-20, 5, 0), glm::vec3(0.f), glm::vec3(0.f));
	vs1.AddForce(Gravity(g));
	vs1.AddForce(Draft(glm::vec3(c)));
	VerletSystem vs2(dt, m, glm::vec3(0, 5, 0), glm::vec3(0.f), glm::vec3(0.f));
	vs2.AddForce(Gravity(g));
	vs2.AddForce(Draft(glm::vec3(c)));
	VerletSystem vs3(dt, m, glm::vec3(20, 5, 0), glm::vec3(0.f), glm::vec3(0.f));
	vs3.AddForce(Gravity(g));
	vs3.AddForce(Draft(glm::vec3(c)));

	VerletSystem vs4(dt, m, glm::vec3(-30, 20, 0), glm::vec3(0.f), glm::vec3(0.f));
	vs4.AddForce(Gravity(g));
	vs4.AddForce(Draft(glm::vec3(c)));
	VerletSystem vs5(dt, m, glm::vec3(-10, 20, 0), glm::vec3(0.f), glm::vec3(0.f));
	vs5.AddForce(Gravity(g));
	vs5.AddForce(Draft(glm::vec3(c)));
	VerletSystem vs6(dt, m, glm::vec3(10, 20, 0), glm::vec3(0.f), glm::vec3(0.f));
	vs6.AddForce(Gravity(g));
	vs6.AddForce(Draft(glm::vec3(c)));
	VerletSystem vs7(dt, m, glm::vec3(30, 20, 0), glm::vec3(0.f), glm::vec3(0.f));
	vs7.AddForce(Gravity(g));
	vs7.AddForce(Draft(glm::vec3(c)));

	float bl = glm::distance(vs1.GetPosition(), vs2.GetPosition());
	float tl = glm::distance(vs4.GetPosition(), vs5.GetPosition());

	// Left anchor to top and bottom
	Rope blrp (k, bl, lss, vs1);
	Rope tlrp (k, glm::distance(lss.GetPosition(), vs4.GetPosition()), lss, vs4);

	// Link between bottom system
	Rope brp1(k, bl, vs1, vs2);
	Rope brp2(k, bl, vs2, vs3);

	// Link between top system
	Rope trp1(k, tl, vs4, vs5);
	Rope trp2(k, tl, vs5, vs6);
	Rope trp3(k, tl, vs6, vs7);

	// Vertical links
//	Rope v1(k, l, vs1, vs4);
//	Rope v2(k, l, vs2, vs5);
//	Rope v3(k, l, vs3, vs6);

	float cl = glm::distance(vs1.GetPosition(), vs4.GetPosition());
	Rope crp1(k, cl, vs4, vs1);

	Rope crp3(k, cl, vs5, vs1);
	Rope crp2(k, cl, vs5, vs2);

	Rope crp4(k, cl, vs6, vs2);
	Rope crp5(k, cl, vs6, vs3);

	Rope crp6(k, cl, vs7, vs3);

	// Right anchor to top and bottom
	Rope brrp (k, bl, vs3, rss);
	Rope trrp (k, glm::distance(rss.GetPosition(), vs7.GetPosition()), vs7, rss);

	SetInterval(stopPhysics, dt_ms, [&]{
		blrp.Tick(dt);
		tlrp.Tick(dt);

		brp1.Tick(dt);
		brp2.Tick(dt);

		trp1.Tick(dt);
		trp2.Tick(dt);
		trp3.Tick(dt);

		crp1.Tick(dt);
		crp2.Tick(dt);
		crp3.Tick(dt);
		crp4.Tick(dt);
		crp5.Tick(dt);
		crp6.Tick(dt);

		brrp.Tick(dt);
		trrp.Tick(dt);

		vs1.Tick(dt);
		vs2.Tick(dt);
		vs3.Tick(dt);
		vs4.Tick(dt);
		vs5.Tick(dt);
		vs6.Tick(dt);
		vs7.Tick(dt);

		s1.get_transform().set_position(GlmToVec(vs1.GetPosition()));
		s2.get_transform().set_position(GlmToVec(vs2.GetPosition()));
		s3.get_transform().set_position(GlmToVec(vs3.GetPosition()));
		s4.get_transform().set_position(GlmToVec(vs4.GetPosition()));
		s5.get_transform().set_position(GlmToVec(vs5.GetPosition()));
		s6.get_transform().set_position(GlmToVec(vs6.GetPosition()));
		s7.get_transform().set_position(GlmToVec(vs7.GetPosition()));

//		stopPhysics.store(true);
	});

	lan.get_transform().set_position(GlmToVec(lss.GetPosition()));
	ran.get_transform().set_position(GlmToVec(rss.GetPosition()));

	while (!w->is_close())
	{
		w->clear(ClearMask::COLOR | ClearMask::DEPTH);

		w->draw(lan);
		w->draw(ran);
		w->draw(s1);
		w->draw(s2);
		w->draw(s3);
		w->draw(s4);
		w->draw(s5);
		w->draw(s6);
		w->draw(s7);

		w->display();
	}

	return 0;
}