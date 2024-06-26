#pragma once

#include "../Entity.hpp"
#include "../Files/PMXFile.hpp"
#include "../Animation/Animation.hpp"

#include "Armature.hpp"
#include "Skin.hpp"
#include "Morph.hpp"

namespace mm
{
	class Renderer;
	class World;
	class ModelPhysicsData;

	class Model : public Entity
	{
		friend class Renderer;
		friend class Armature;
		friend class Skin;
		friend class Morph;

		static constexpr uint32_t SKINNING_SSBO_BASE = 1;
		static constexpr uint32_t MORPH_VERTEX_SSBO_BASE = 2;

	public:
		Model(World& world, const std::filesystem::path& path);
		Animation* LoadAnimation(const std::filesystem::path& path);
		//void Render(Renderer& renderer);
		void Update(float deltaTime);

		const PMXFile& GetPMXFile() const { return *m_pmxFile; }
		Armature& GetArmature() { return *m_armature; }
		Morph& GetMorph() { return *m_morph; }
		const Morph& GetMorph() const { return *m_morph; }
		Skin& GetSkin() { return *m_skin; }
		Animation& GetAnim() { return *m_animation; }

		void SyncWithPhysics();
		void CalcSkinning() { m_armature->CalcSkinning(); }

	private:
		World& m_world;
		PMXFile* m_pmxFile;

		// ---------- COMPONENTS ----------
		std::unique_ptr<Animation> m_animation;
		std::unique_ptr<Armature> m_armature;
		std::unique_ptr<Skin> m_skin;
		std::unique_ptr<Morph> m_morph;

		// ---------- BUFFERS ----------
		std::unique_ptr<GLBuffer> m_skinningBuffer;
		std::unique_ptr<GLBuffer> m_vertexMorphBuffer;
		//std::vector<MaterialLayout> m_materialMorphBuffer;

		// ---------- PHYSICS ----------
		ModelPhysicsData* m_physicsData;
	};
}

