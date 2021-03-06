/*
 *   CrissCross
 *   A multi-purpose cross-platform library.
 *
 *   A product of Uplink Laboratories.
 *
 *   (c) 2006-2009 Steven Noonan.
 *   Licensed under the New BSD License.
 *
 */

#ifndef __included_cc_stltree_h
#define __included_cc_stltree_h


/*
 * Something is wrong with Microsoft's and
 * Borland's std::map implementation.
 */
#ifdef ENABLE_STLTREE

#include <crisscross/deprecate.h>
#include <crisscross/compare.h>
#include <crisscross/node.h>
#include <crisscross/internal_mem.h>

#include <map>

namespace CrissCross
{
	namespace Data
	{
		/*! \brief An STL map wrapper. */
		/*!
		 *      This is a tree which does NOT allow duplicate keys.
		 */
		template <class Key, class Data>
		class STLTree
		{
			private:
				std::map<Key,Data,CrissCross::Data::LessThanComparator<Key> > m_map;

			public:

				/*! \brief The default constructor. */
				STLTree() {};

				/*! \brief The destructor. */
				~STLTree() { empty(); };

				/*! \brief Empties the entire tree. */
				/*!
				 * \warning This won't free the memory occupied by the data, so the data must be freed
				 *    separately. The preferred way to do this is to serialize the data into a DArray
				 *    with ConvertToDArray() and then iterate through it to delete the data in whatever
				 *    way is proper.
				 */
				__forceinline void empty()
				{
					typename std::map<Key,Data,CrissCross::Data::LessThanComparator<Key> >::iterator iter;
					for (iter = m_map.begin(); iter != m_map.end(); iter++) {
						Dealloc(iter->first);
					}
					m_map.clear();
				};

				/*! \brief Inserts data into the tree. */
				/*!
				 * \param _key The key of the data.
				 * \param _rec The data to insert.
				 * \return True on success, false on failure.
				 */
				__forceinline bool insert(Key const &_key, Data const &_rec)
				{
					if (exists(_key))
						return false;
					m_map[Duplicate(_key)] = _rec;
					return true;
				};

				/*! \brief Tests whether a key is in the tree or not. */
				/*!
				 * \param _key The key of the node to find.
				 * \return True if the key is in the tree, false if not.
				 */
				__forceinline bool exists(Key const &_key) const
				{ return m_map.count(_key) != 0; };

				/*! \brief Change the data at the given node. */
				/*!
				 * \param _key The key of the node to be modified.
				 * \param _rec The data to insert.
				 * \return True on success, false on failure.
				 */
				__forceinline bool replace(Key const &_key, Data const &_rec)
				{
					if (!exists(_key))
						return false;
					m_map[_key] = _rec;
					return true;
				};

				/*! \brief Finds a node in the tree and returns the data at that node. */
				/*!
				 * \param _key The key of the node to find.
				 * \param _default The value to return if the item couldn't be found.
				 * \return If found, returns the data at the node, otherwise _default is returned.
				 */
				__forceinline Data find(Key const &_key, Data const &_default = NULL) const
				{
					if (!exists(_key))
						return _default;
					typename std::map<Key,Data,CrissCross::Data::LessThanComparator<Key> >::const_iterator iter;
					iter = m_map.find(_key);
					return iter->second;
				};

				/*! \brief Deletes a node from the tree, specified by the node's key. */
				/*!
				 * \warning This won't free the memory occupied by the data, so the data must be freed separately.
				 * \param _key The key of the node to delete.
				 * \return True on success, false on failure
				 */
				__forceinline bool erase(Key const &_key)
				{
					typename std::map<Key,Data,CrissCross::Data::LessThanComparator<Key> >::iterator iter;
					iter = m_map.find(_key);
					if (iter == m_map.end())
						return false;
					Key key = iter->first;
					m_map.erase(iter);
					Dealloc(key);
					return true;
				};

				/*! \brief Indicates the size of the tree. */
				/*!
				 * \return Size of the tree.
				 */
				__forceinline size_t size() const
				{ return m_map.size(); };

				/*! \brief Converts the tree data into a linearized DArray. */
				/*!
				 * \return A DArray containing the data of the tree.
				 * \warning Delete the returned DArray when done with it.
				 */
				__forceinline DArray <Data> *ConvertToDArray() const
				{
					DArray<Data> *darray = new DArray<Data>(size());
					typename std::map<Key,Data,CrissCross::Data::LessThanComparator<Key> >::const_iterator iter;
					for (iter = m_map.begin(); iter != m_map.end(); iter++) {
						darray->insert(iter->second);
					}
					return darray;
				};

				/*! \brief Converts the tree keys into a linearized DArray. */
				/*!
				 * \return A DArray containing the keys in the tree.
				 * \warning Delete the returned DArray when done with it.
				 */
				__forceinline DArray <Key> *ConvertIndexToDArray() const
				{
					DArray<Key> *darray = new DArray<Key>(size());
					typename std::map<Key,Data,CrissCross::Data::LessThanComparator<Key> >::const_iterator iter;
					for (iter = m_map.begin(); iter != m_map.end(); iter++) {
						darray->insert(iter->first);
					}
					return darray;
				};

#if !defined (DISABLE_DEPRECATED_CODE)
				/*
				 *      Deprecated Compatibility Functions
				 *      Provided for compatibility with Tosser I
				 */
				/*! @cond */
				_CC_DEPRECATE_FUNCTION(insert) __forceinline void PutData(Key const &_key, Data const & _rec)
				{
					insert(_key, _rec);
				};
				_CC_DEPRECATE_FUNCTION(find)   __forceinline Data GetData(Key const &_key, Data const &_default = NULL)
				{
					return find(_key, _default);
				};
				_CC_DEPRECATE_FUNCTION(erase)  __forceinline void RemoveData(Key const &_key)
				{
					erase(_key);
				};
				_CC_DEPRECATE_FUNCTION(erase)  __forceinline void RemoveData(Key const &_key, Data const & _rec)
				{
					erase(_key, _rec);
				};
				_CC_DEPRECATE_FUNCTION(size)   __forceinline int  Size() const
				{
					return (int)size();
				};
				_CC_DEPRECATE_FUNCTION(empty)  __forceinline void Empty()
				{
					empty();
				};
				/*! @endcond */
#endif
		};
	}
}

#endif
#endif
