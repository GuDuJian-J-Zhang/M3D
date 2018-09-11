#include "m3d/scene/Octree.h"

#ifdef _MSC_VER
#pragma warning(disable:4355)
#endif
#include <algorithm>
namespace M3D
{

static const float DEFAULT_OCTREE_SIZE = 1000.0f;
static const int DEFAULT_OCTREE_LEVELS = 6;
static const int RAYCASTS_PER_WORK_ITEM = 4;

extern const char* SUBSYSTEM_CATEGORY;

//void RaycastDrawablesWork(const WorkItem* item, unsigned threadIndex)
//{
//    Octree* octree = reinterpret_cast<Octree*>(item->aux_);
//    LModelShape** start = reinterpret_cast<LModelShape**>(item->start_);
//    LModelShape** end = reinterpret_cast<LModelShape**>(item->end_);
//    const RayOctreeQuery& query = *octree->m_rayQuery;
//    vector<RayQueryResult>& results = octree->m_rayQueryResults[threadIndex];
//
//    while (start != end)
//    {
//        LModelShape* drawable = *start;
//        drawable->ProcessRayQuery(query, results);
//        ++start;
//    }
//}

void UpdateDrawablesWork(/*const WorkItem* item, unsigned threadIndex*/)
{
 //   const FrameInfo& frame = *(reinterpret_cast<FrameInfo*>(item->aux_));
	//LModelShape** start = reinterpret_cast<LModelShape**>(item->start_);
	//LModelShape** end = reinterpret_cast<LModelShape**>(item->end_);

 //   while (start != end)
 //   {
	//	LModelShape* drawable = *start;
 //       if (drawable)
 //           drawable->Update(frame);
 //       ++start;
 //   }
}

inline bool CompareRayQueryResults(const RayQueryResult& lhs, const RayQueryResult& rhs)
{
    return lhs.m_distance < rhs.m_distance;
}

Octant::Octant(const BoundingBox& box, unsigned level, Octant* parent, Octree* root, unsigned index) :
    m_level(level),
    m_numDrawables(0),
    m_parent(parent),
    m_root(root),
    m_index(index),
	m_dirty(true)
{
    Initialize(box);

    for (unsigned i = 0; i < NUM_OCTANTS; ++i)
        m_children[i] = 0;
}

Octant::~Octant()
{
    if (m_root)
    {
        // Remove the drawables (if any) from this octant to the root octant
        for (vector<ModelShape*>::iterator i = m_drawables.begin(); i != m_drawables.end(); ++i)
        //{
        //    (*i)->SetOctant(m_root);
        //    m_root->m_drawables.push_back(*i);
        //    m_root->QueueUpdate(*i);
        //}
        m_drawables.clear();
        m_numDrawables = 0;
    }

    for (unsigned i = 0; i < NUM_OCTANTS; ++i)
        DeleteChild(i);
}

Octant* Octant::GetOrCreateChild(unsigned index)
{
    if (m_children[index])
        return m_children[index];

    Vector3 newMin = m_worldBoundingBox.m_min;
    Vector3 newMax = m_worldBoundingBox.m_max;
    Vector3 oldCenter = m_worldBoundingBox.Center();

    if (index & 1)
        newMin.m_x = oldCenter.m_x;
    else
        newMax.m_x = oldCenter.m_x;

    if (index & 2)
        newMin.m_y = oldCenter.m_y;
    else
        newMax.m_y = oldCenter.m_y;

    if (index & 4)
        newMin.m_z = oldCenter.m_z;
    else
        newMax.m_z = oldCenter.m_z;

    m_children[index] = new Octant(BoundingBox(newMin, newMax), m_level + 1, this, m_root, index);
    return m_children[index];
}

void Octant::DeleteChild(unsigned index)
{
    assert(index < NUM_OCTANTS);
	if (m_children[index])
	{
		delete m_children[index];
		m_children[index] = 0;
	}
}

void Octant::InsertDrawable(ModelShape* drawable)
{
    const BoundingBox& box = drawable->GetWorldBoundingBox();

    // If root octant, insert all non-occludees here, so that octant occlusion does not hide the drawable.
    // Also if drawable is outside the root octant bounds, insert to root
    bool insertHere;
	//
	if (!drawable->AllowCuller())
	{
		insertHere = true;
	}
	else if (this == m_root) {
		insertHere = /*!drawable->IsOccludee() || */m_cullingBox.IsInside(box) != INSIDE || CheckDrawableFit(box);
	}
	else {
		insertHere = CheckDrawableFit(box);
	}

    if (insertHere)
    {
        Octant* oldOctant = drawable->GetOctant();
        if (oldOctant != this)
        {
            // Add first, then remove, because drawable count going to zero deletes the octree branch in question
            AddDrawable(drawable);
            if (oldOctant)
                oldOctant->RemoveDrawable(drawable, false);
        }
    }
    else
    {
        Vector3 boxCenter = box.Center();
        unsigned x = boxCenter.m_x < m_center.m_x ? 0 : 1;
        unsigned y = boxCenter.m_y < m_center.m_y ? 0 : 2;
        unsigned z = boxCenter.m_z < m_center.m_z ? 0 : 4;

        GetOrCreateChild(x + y + z)->InsertDrawable(drawable);
    }
	m_dirty = true;
}

void Octant::RemoveAllDrawables()
{
	for (vector<ModelShape*>::iterator i = m_drawables.begin(); i != m_drawables.end(); ++i)
		(*i)->SetOctant(0);
}

bool Octant::CheckDrawableFit(const BoundingBox& box) const
{
    Vector3 boxSize = box.Size();

    // If max split level, size always OK, otherwise check that box is at least half size of octant
    if (m_level >= m_root->GetNumLevels() || boxSize.m_x >= m_halfSize.m_x || boxSize.m_y >= m_halfSize.m_y ||
        boxSize.m_z >= m_halfSize.m_z)
        return true;
    // Also check if the box can not fit a child octant's culling box, in that case size OK (must insert here)
    else
    {
        if (box.m_min.m_x <= m_worldBoundingBox.m_min.m_x - 0.5f * m_halfSize.m_x ||
            box.m_max.m_x >= m_worldBoundingBox.m_max.m_x + 0.5f * m_halfSize.m_x ||
            box.m_min.m_y <= m_worldBoundingBox.m_min.m_y - 0.5f * m_halfSize.m_y ||
            box.m_max.m_y >= m_worldBoundingBox.m_max.m_y + 0.5f * m_halfSize.m_y ||
            box.m_min.m_z <= m_worldBoundingBox.m_min.m_z - 0.5f * m_halfSize.m_z ||
            box.m_max.m_z >= m_worldBoundingBox.m_max.m_z + 0.5f * m_halfSize.m_z)
            return true;
    }

    // Bounding box too small, should create a child octant
    return false;
}

void Octant::ResetRoot()
{
    m_root = 0;

    // The whole octree is being destroyed, just detach the drawables
    for (vector<ModelShape*>::iterator i = m_drawables.begin(); i != m_drawables.end(); ++i)
        (*i)->SetOctant(0);

    for (unsigned i = 0; i < NUM_OCTANTS; ++i)
    {
        if (m_children[i])
            m_children[i]->ResetRoot();
    }
}


void  Octant::Clear()
{
	// The whole octree is being destroyed, just detach the drawables
	for (vector<ModelShape*>::iterator i = m_drawables.begin(); i != m_drawables.end(); ++i)
		(*i)->SetOctant(0);

	m_drawables.resize(0);

	for (unsigned i = 0; i < NUM_OCTANTS; ++i)
	{
		if (m_children[i])
			m_children[i]->Clear();
	}
}

void Octant::Initialize(const BoundingBox& box)
{
    m_worldBoundingBox = box;
    m_center = box.Center();
    m_halfSize = 0.5f * box.Size();
    m_cullingBox = BoundingBox(m_worldBoundingBox.m_min - m_halfSize, m_worldBoundingBox.m_max + m_halfSize);
}

bool Octant::CompareByPlcWorldBoxLength(ModelShape* modelShape1,
	ModelShape* modelShape2)
{
	float length1 = modelShape1->GetWorldBoundingBox().Length();
	if (!modelShape1->AllowCuller())
	{
		length1 = 1000000 + modelShape1->GetModel()->GetID();
	}
	
	float length2 = modelShape2->GetWorldBoundingBox().Length();
	if (!modelShape2->AllowCuller())
	{
		length2 = 1000000 + modelShape2->GetModel()->GetID();
	}

	return length1 > length2;
}

void Octant::GetDrawablesInternal(OctreeQuery& query, bool inside) const
{
    if (this != m_root)
    {
        Intersection res = query.TestOctant(m_cullingBox, inside);
        if (res == INSIDE)
            inside = true;
        else if (res == OUTSIDE)
        {
            //Fully outside, so cull this octant, its children & drawables
            return;
        }
    }

    if (m_drawables.size())
    {
		if (this->m_dirty)
		{
			this->m_dirty = false;
			//����drawables
			sort(this->m_drawables.begin(), this->m_drawables.end(),
				CompareByPlcWorldBoxLength);
		}
		ModelShape** start = const_cast<ModelShape**>(&m_drawables[0]);
		ModelShape** end = start + m_drawables.size();
        query.TestDrawables(start, end, inside);
    }

    for (unsigned i = 0; i < NUM_OCTANTS; ++i)
    {
        if (m_children[i])
            m_children[i]->GetDrawablesInternal(query, inside);
    }
}

void Octant::GetDrawablesInternal(RayOctreeQuery& query) const
{
    float octantDist = query.m_ray.HitDistance(m_cullingBox);
    if (octantDist >= query.m_maxDistance)
        return;

    if (m_drawables.size())
    {
		ModelShape** start = const_cast<ModelShape**>(&m_drawables[0]);
		ModelShape** end = start + m_drawables.size();

        while (start != end)
        {
			ModelShape* drawable = *start++;
			//�˲��� �������㷨
           // if ((drawable->GetDrawableFlags() & query.m_drawableFlags) && (drawable->GetViewMask() & query.m_viewMask))
           //     drawable->ProcessRayQuery(query, query.m_result);
        }
    }

    for (unsigned i = 0; i < NUM_OCTANTS; ++i)
    {
        if (m_children[i])
            m_children[i]->GetDrawablesInternal(query);
    }
}

void Octant::GetDrawablesOnlyInternal(RayOctreeQuery& query, vector<ModelShape*>& drawables) const
{
    float octantDist = query.m_ray.HitDistance(m_cullingBox);
    if (octantDist >= M_INFINITY)
        return;

    if (m_drawables.size())
    {
		ModelShape** start = const_cast<ModelShape**>(&m_drawables[0]);
		ModelShape** end = start + m_drawables.size();
        while (start != end)
        {
			ModelShape* drawable = *start++;
           // if ((drawable->GetDrawableFlags() & query.m_drawableFlags) && (drawable->GetViewMask() & query.m_viewMask))
			if (drawable->IsVisible())
			{
				drawables.push_back(drawable);
			}
        }
    }

    for (unsigned i = 0; i < NUM_OCTANTS; ++i)
    {
        if (m_children[i])
            m_children[i]->GetDrawablesOnlyInternal(query, drawables);
    }
}

Octree::Octree() :
    Octant(BoundingBox(-DEFAULT_OCTREE_SIZE, DEFAULT_OCTREE_SIZE), 0, 0, this),
    m_numLevels(DEFAULT_OCTREE_LEVELS)
{
    // Resize threaded ray query intermediate result vector according to number of worker threads
    //WorkQueue* workQueue = GetSubsystem<WorkQueue>();
    //m_rayQueryResults.Resize(workQueue ? workQueue->GetNumThreads() + 1 : 1);

    // If the engine is running headless, subscribe to RenderUpdate events for manually updating the octree
    // to allow raycasts and animation update
    //if (!GetSubsystem<Graphics>())
    //    SubscribeToEvent(E_RENDERUPDATE, URHO3D_HANDLER(Octree, HandleRenderUpdate));
}

Octree::~Octree()
{
    // Reset root pointer from all child octants now so that they do not move their drawables to root
    m_drawableUpdates.clear();
    m_drawableReinsertions.clear();
    ResetRoot();
}

void Octree::Clear()
{
	m_drawableUpdates.clear();
	m_drawableReinsertions.clear();
	//ResetRoot();

	Octant::Clear();
	for (unsigned i = 0; i < NUM_OCTANTS; ++i) {
		if (m_children[i])
		{
			m_children[i]->Clear();
			delete m_children[i];
			m_children[i] = 0;
		}
	}
}
 
void Octree::SetSize(const BoundingBox& box, unsigned numLevels)
{
    // If drawables exist, they are temporarily moved to the root
    for (unsigned i = 0; i < NUM_OCTANTS; ++i)
        DeleteChild(i);

    Initialize(box);
    m_numDrawables = m_drawables.size();
    m_numLevels = (unsigned)Max((int)numLevels, 1);
}

void Octree::Update()
{
    // Reinsert drawables that have been moved or resized, or that have been newly added to the octree and do not sit inside
    // the proper octant yet
    if (!m_drawableUpdates.empty())
    {
        for (int i =0;i<m_drawableUpdates.size(); ++i)
        {
			ModelShape* drawable = m_drawableUpdates.at(i);
            //drawable->updateQueued_ = false;
            Octant* octant = drawable->GetOctant();
            const BoundingBox& box = drawable->GetWorldBoundingBox();

            // Skip if no octant or does not belong to this octree anymore
            if (!octant || octant->GetRoot() != this)
                continue;
            // Skip if still fits the current octant
            if (/*drawable->IsOccludee() && */octant->GetCullingBox().IsInside(box) == INSIDE && octant->CheckDrawableFit(box))
                continue;

            InsertDrawable(drawable);
        }

		m_drawableUpdates.clear();
    }
}

void Octree::AddManualDrawable(ModelShape* drawable)
{
    if (!drawable || drawable->GetOctant())
        return;

    AddDrawable(drawable);
}

void Octree::RemoveManualDrawable(ModelShape* drawable)
{
    if (!drawable)
        return;

    Octant* octant = drawable->GetOctant();
    if (octant && octant->GetRoot() == this)
        octant->RemoveDrawable(drawable);
}

void Octree::GetDrawables(OctreeQuery& query) const
{
    query.m_result.clear();
    GetDrawablesInternal(query, false);
}

void Octree::Raycast(RayOctreeQuery& query) const
{
   
}

void Octree::RaycastSingle(RayOctreeQuery& query) const
{
	query.m_result->clear();
	m_rayQuery = &query;
	m_rayQueryDrawables.clear();
	GetDrawablesOnlyInternal(query, m_rayQueryDrawables);

	query.m_fastResult->insert(query.m_fastResult->end(),
		m_rayQueryDrawables.begin(), m_rayQueryDrawables.end());
}

void Octree::RaycastSingleFast(RayOctreeQuery& query) const
{
	query.m_fastResult->clear();
	m_rayQuery = &query;
	m_rayQueryDrawables.clear();
	GetDrawablesOnlyInternal(query, m_rayQueryDrawables);

	query.m_fastResult->insert(query.m_fastResult->end(),
		m_rayQueryDrawables.begin(), m_rayQueryDrawables.end());
}

void Octree::QueueUpdate(ModelShape* drawable)
{
	if (!drawable->isOCtreeUpdated())
	{
		m_drawableUpdates.push_back(drawable);
		drawable->SetOCTreeUpdated(true);
	}
}

}
