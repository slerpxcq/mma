#include "mmpch.hpp"
#include "ModelNode.hpp"

#include "Core/Manager/FileManager.hpp"

#include "ArmatureNode.hpp"
#include "SkinNode.hpp"
#include "MorphNode.hpp"

namespace mm
{

/* ModelNode factory */
std::unique_ptr<ModelNode> ModelNode::Make(const PMXFile& pmx)
{
    auto model = std::make_unique<ModelNode>();

    /* Load components */
    /* Armature */
    auto armature = ArmatureNode::Make(pmx);
    armature->m_parent = model.get();
    model->m_armatureNode = armature.get();
    model->m_children.push_back(std::move(armature));
    /* Skin */
    auto skin = SkinNode::Make(pmx);
    skin->m_parent = model.get();
    model->m_skinNode = skin.get();
    model->m_children.push_back(std::move(skin));
    /* Morph */
    auto morph = MorphNode::Make(pmx);
    morph->m_parent = model.get();
    model->m_morphNode = morph.get();
    model->m_children.push_back(std::move(morph));

    return model;
}

} // namespace mm

