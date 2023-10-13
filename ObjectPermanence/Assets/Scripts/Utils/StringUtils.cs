using System.Text;
using UnityEngine;

namespace ObjectPermanence
{
    /**
     * A collection of utility functions for managing strings.
     */
    public static class StringUtils
    {
        public static string BuildString(params string[] strings)
        {
            var stringBuilder = new StringBuilder();
            foreach (string str in strings)
            {
                stringBuilder.Append(str);
            }

            return stringBuilder.ToString();
        }

        public static string ColourString(string str, Color colour)
        {
            string colourAsString = ColorUtility.ToHtmlStringRGBA(colour);
            string startColourTag = $"<color=#{colourAsString}>";
            const string endColourTag = "</color>";

            return $"{startColourTag}{str}{endColourTag}";
        }
    }
}
