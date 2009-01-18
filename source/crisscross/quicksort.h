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

#ifndef __included_cc_quicksort_h
#define __included_cc_quicksort_h

#ifdef ENABLE_SORTS

#include <crisscross/deprecate.h>
#include <crisscross/sort.h>
#include <crisscross/heapsort.h>

namespace CrissCross
{
	namespace Data
	{
		/*! \brief A QuickSort implementation for sorting arrays. */
		template <class T>
		class QuickSort : public Sorter<T>
		{
			private:
				HeapSort<T> hs;
				void InternalSort(T *_array, size_t _left, size_t _right)
				{
					int i, j, p, p1, p2, p3;

					if (_right > _left) {
						if (_right - _left < 10)
							hs.Sort(&_array[_left], _right - _left + 1);
						else {
							i = _left - 1;
							j = _right;

							/* Pivot zufaellig waehlen */
							p1 = _left + ((lrand48()) % (_right - _left + 1));
							p2 = _left + ((lrand48()) % (_right - _left + 1));
							p3 = _left + ((lrand48()) % (_right - _left + 1));
							if (Compare(_array[p1], _array[p2]) < 0)
								if (Compare(_array[p2], _array[p3]) < 0) {
									p = p2;
								} else {
									if (Compare(_array[p1], _array[p3]) < 0)
										p = p3;
									else
										p = p1;
								}
							else
							if (Compare(_array[p2], _array[p3]) > 0) {
								p = p2;
							} else {
								if (Compare(_array[p1], _array[p3]) > 0)
									p = p3;
								else
									p = p1;
							}

							Swap(_array, p, _right);

							for (;;) {
								do i++; while (Compare(_array[i], _array[_right]) < 0);
								do j--; while (Compare(_array[j], _array[_right]) > 0);

								if (i >= j) break;

								Swap(_array, i, j);
							}

							Swap(_array, i, _right);

							if (i - 1 - _left < _right - i + 1) {
								InternalSort(_array, _left, i - 1);
								InternalSort(_array, i + 1, _right);
							} else {
								InternalSort(_array, i + 1, _right);
								InternalSort(_array, _left, i - 1);
							}
						}
					}
				}

			public:

				/*! \brief Sorts an array using the QuickSort method. */
				/*!
				 * \param _array The array to sort.
				 * \param _size The size of the array to sort.
				 * \return 0 on success.
				 * \sa HeapSort ShellSort CombSort
				 */
				int Sort(T *_array, size_t _size)
				{
					for (size_t i = _size - 1; i > 0; i--) {
						if (Compare(_array[i - 1], _array[i]) > 0)
							Swap(_array, i - 1, i);
					}
					InternalSort(_array, 1, _size - 1);
					return 0;
				}
		};
	}
}

#endif

#endif
