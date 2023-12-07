using System;

namespace ObjectPermanence
{
    /**
     * Some basic enum extensions 
     */
    public static class EnumUtils
    {
        public static T Next<T>(this T _enum) where T : Enum
        {
            T[] enumAsArray = (T[])Enum.GetValues(_enum.GetType());
            int nextIndex = Array.IndexOf(enumAsArray, _enum) + 1;
            return (enumAsArray.Length == nextIndex) ? enumAsArray[0] : enumAsArray[nextIndex];
        }
    }
}
