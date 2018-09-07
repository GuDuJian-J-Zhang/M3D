#ifndef M3D_OCTREE_H_
#define M3D_OCTREE_H_

#include "m3d/M3D.h"
#include "m3d/model/ModelShape.h"
#include "m3d/scene/OctreeQuery.h"

namespace M3D
{

class Octree;

static const int NUM_OCTANTS = 8;
static const unsigned ROOT_INDEX = M_MAX_UNSIGNED;

/// %Octree octant
class M3D_API Octant
{
public:
    /// Construct.
    Octant(const BoundingBox& box, unsigned level, Octant* parent, Octree* root, unsigned index = ROOT_INDEX);
    /// Destruct. Move drawables to root if available (detach if not) and free child octants.
    virtual ~Octant();

    /// Return or create a child octant.
    Octant* GetOrCreateChild(unsigned index);
    /// Delete child octant.
    void DeleteChild(unsigned index);
    /// Insert a LModelShape object by checking for fit recursively.
    void InsertDrawable(ModelShape* drawable);
    /// Check if a drawable object fits.
    bool CheckDrawableFit(const BoundingBox& box) const;

	void RemoveAllDrawables();

    /// Add a drawable object to this octant.
    void AddDrawable(ModelShape* drawable)
    {
		drawable->SetOctant(this);
        m_drawables.push_back(drawable);
        IncDrawableCount();
    }

    /// Remove a drawable object from this octant.
    void RemoveDrawable(ModelShape* drawable, bool resetOctant = true)
    {
		bool removed = false;
		vector<ModelShape*>::iterator it = m_drawables.begin();
		while (it != m_drawables.end())
		{
			if (*it == drawable)
			{
				m_drawables.erase(it);
				removed = true;
				break;
			}
			it++;
		}
        if (removed)
        {
            if (resetOctant)
                drawable->SetOctant(0);
            DecDrawableCount();
        }

		m_dirty = true;
    }

    /// Return world-space bounding box.
    const BoundingBox& GetWorldBoundingBox() const { return m_worldBoundingBox; }

    /// Return bounding box used for fitting drawable objects.
    const BoundingBox& GetCullingBox() const { return m_cullingBox; }

    /// Return subdivision level.
    unsigned GetLevel() const { return m_level; }

    /// Return parent octant.
    Octant* GetParent() const { return m_parent; }

    /// Return octree root.
    Octree* GetRoot() const { return m_root; }

    /// Return number of drawables.
    unsigned GetNumDrawables() const { return m_numDrawables; }

    /// Return true if there are no drawable objects in this octant and child octants.
    bool IsEmpty() { return m_numDrawables == 0; }

    /// Reset root pointer recursively. Called when the whole octree is being destroyed.
    void ResetRoot();

	virtual void Clear();

private:
	static bool CompareByPlcWorldBoxLength(ModelShape* node1, ModelShape* node2);
protected:
    /// Initialize bounding box.
    void Initialize(const BoundingBox& box);
    /// Return drawable objects by a query, called internally.
    void GetDrawablesInternal(OctreeQuery& query, bool inside) const;
    /// Return drawable objects by a ray query, called internally.
    void GetDrawablesInternal(RayOctreeQuery& query) const;
    /// Return drawable objects only for a threaded ray query, called internally.
    void GetDrawablesOnlyInternal(RayOctreeQuery& query, vector<ModelShape*>& drawables) const;

    /// Increase drawable object count recursively.
    void IncDrawableCount()
    {
        ++m_numDrawables;
        if (m_parent)
            m_parent->IncDrawableCount();
    }

    /// Decrease drawable object count recursively and remove octant if it becomes empty.
    void DecDrawableCount()
    {
        Octant* parent = m_parent;

        --m_numDrawables;
        if (!m_numDrawables)
        {
            if (parent)
                parent->DeleteChild(m_index);
        }

        if (parent)
            parent->DecDrawableCount();
    }

    /// World bounding box.
    BoundingBox m_worldBoundingBox;
    /// Bounding box used for drawable object fitting.
    BoundingBox m_cullingBox;
    /// Drawable objects.
    mutable vector<ModelShape*> m_drawables;
    /// Child octants.
    Octant* m_children[NUM_OCTANTS];
    /// World bounding box center.
    Vector3 m_center;
    /// World bounding box half size.
    Vector3 m_halfSize;
    /// Subdivision level.
    unsigned m_level;
    /// Number of drawable objects in this octant and child octants.
    unsigned m_numDrawables;
    /// Parent octant.
    Octant* m_parent;
    /// Octree root.
    Octree* m_root;
    /// Octant index relative to its siblings or ROOT_INDEX for root octant
    unsigned m_index;

	mutable bool m_dirty;
};

/// %Octree component. Should be added only to the root scene node
class M3D_API Octree :public Octant
{
    //friend void RaycastDrawablesWork(const WorkItem* item, unsigned threadIndex);
public:
    /// Construct.
    Octree();
    /// Destruct.
    ~Octree();

	virtual void Clear();

    /// Set size and maximum subdivision levels. If octree is not empty, drawable objects will be temporarily moved to the root.
    void SetSize(const BoundingBox& box, unsigned numLevels);
    /// Update and reinsert drawable objects.
    void Update(/*const FrameInfo& frame*/);
    /// Add a drawable manually.
    void AddManualDrawable(ModelShape* drawable);
    /// Remove a manually added drawable.
    void RemoveManualDrawable(ModelShape* drawable);

    /// Return drawable objects by a query.
    void GetDrawables(OctreeQuery& query) const;
    /// Return drawable objects by a ray query.
    void Raycast(RayOctreeQuery& query) const;
    /// Return the closest drawable object by a ray query.
    void RaycastSingle(RayOctreeQuery& query) const;

	void RaycastSingleFast(RayOctreeQuery& query) const;

    /// Return subdivision levels.
    unsigned GetNumLevels() const { return m_numLevels; }

	/// Mark drawable object as requiring an update and a reinsertion.
	void QueueUpdate(ModelShape* drawable);
private:

    /// Drawable objects that require update.
    vector<ModelShape*> m_drawableUpdates;
    /// Drawable objects that require reinsertion.
	vector<ModelShape*> m_drawableReinsertions;
    /// Mutex for octree reinsertions.
    Mutex m_octreeMutex;
    /// Current threaded ray query.
    mutable RayOctreeQuery* m_rayQuery;
    /// Drawable list for threaded ray query.
    mutable vector<ModelShape*> m_rayQueryDrawables;
    /// Threaded ray query intermediate results.
    mutable vector<vector<RayQueryResult> > m_rayQueryResults;
    /// Subdivision level.
    unsigned m_numLevels;
};

}

#endif
