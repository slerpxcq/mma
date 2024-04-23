#pragma once

#include "../Files/PMXFile.hpp"
#include "../Animation/Animation.hpp"

#include "Armature.hpp"
#include "Skin.hpp"
#include "Morph.hpp"

namespace mm
{
	class GLRenderer;
	class World;
	class ModelPhysicsData;

	class Model
	{
		friend class Armature;
		friend class Skin;
		friend class Morph;

	public:
		Model(World& world, const std::filesystem::path& path);
		Animation* LoadAnimation(const std::filesystem::path& path);
		void Render(GLRenderer& renderer);
		void Update(float deltaTime);
		const PMXFile& GetPMXFile() const { return *m_pmxFile; }
		Armature& GetArmature() { return *m_armature; }
		Morph& GetMorph() { return *m_morph; }
		Skin& GetSkin() { return *m_skin; }
		Animation* GetAnim() { return m_animation.get(); }
		void SyncWithPhysics() { if (m_physicsData != nullptr) m_armature->SyncWithPhysics(*m_physicsData); }
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
		std::vector<MaterialLayout> m_materialMorphBuffer;

		// ---------- PHYSICS ----------
		ModelPhysicsData* m_physicsData;
	};
}

