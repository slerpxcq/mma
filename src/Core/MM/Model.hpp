#pragma once

#include "Files/PMXFile.hpp"
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
		void Render(GLRenderer& renderer);
		void Update(float deltaTime);
		const PMXFile& GetPMXFile() const { return *m_pmxFile; }
		const Armature& GetArmature() const { return *m_armature; }
		void SyncWithPhysics() { if (m_physicsData != nullptr) m_armature->SyncWithPhysics(*m_physicsData); }
		void CalcSkinning() { m_armature->CalcSkinning(); }

	private:
		World& m_world;
		std::unique_ptr<PMXFile> m_pmxFile;

		// ---------- COMPONENTS ----------
		std::unique_ptr<Armature> m_armature;
		std::unique_ptr<Skin> m_skin;
		std::unique_ptr<Morph> m_morph;

		// ---------- RENDERING ----------
		static std::unique_ptr<MMShader> s_meshShader;
		static std::unique_ptr<MMShader> s_morphShader;

		std::unique_ptr<GLBuffer> m_skinningBuffer;
		std::unique_ptr<GLBuffer> m_morphOffsetBuffer;

		// ---------- PHYSICS ----------
		ModelPhysicsData* m_physicsData;
	};
}

