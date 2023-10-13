using System;
using UnityEngine;

namespace ObjectPermanence
{
    /**
     * Allows for easy and nice looking optional variables within the inspector.
     * To use, just wrap a variable in the Optional template. Eg:
     *  public/[SerializeField] Optional<variableType> variableName 
     *      - Create an optional variable of type variableType.
     *  public/[SerializeField] Optional<variableType> variableName = new Optional<variableType>(intialValue) 
     *      - Create an optional variable of type variableType with the value of defaultValue.
     *      
     *  Credit: aarthificial https://www.youtube.com/watch?v=uZmWgQ7cLNI.
     *  See OptionalPropertyDrawer.cs for the editor side of this code.
     */
    [Serializable]
    public class Optional<T>
    {
        public bool Enabled => _enabled;
        public T Value => _value;

        [SerializeField] private bool _enabled;
        [SerializeField] private T _value;

        public Optional(T initial)
        {
            _enabled = true;
            _value = initial;
        }
    }
}
