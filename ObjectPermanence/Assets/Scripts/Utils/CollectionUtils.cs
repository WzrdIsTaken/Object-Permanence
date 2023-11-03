using System;
using System.Collections.Generic;

namespace ObjectPermanence
{
    /**
     * Some functions to manipulate collections.
     */
    public static class CollectionUtils
    {
        public static T Random<T>(this List<T> list)
        {
            Random random = new Random();
            return list[random.Next(0, list.Count - 1)];
        }

        public static void Shuffle<T>(this List<T> list)
        {
            // https://www.geeksforgeeks.org/shuffle-a-given-array-using-fisher-yates-shuffle-algorithm/

            Random random = new Random();
            int n = list.Count;
            for (int i = n - 1; i > 0; i--)
            {
                int j = random.Next(0, i + 1);
                T temp = list[i];
                list[i] = list[j];
                list[j] = temp;
            }
        }
    }
}
