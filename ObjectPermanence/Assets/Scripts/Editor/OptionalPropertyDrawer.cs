using UnityEditor;
using UnityEngine;

namespace ObjectPermanence
{
    /**
     * The editor code to make optional variables (Helper/Optional.cs) look nice in the inspector.
     * 
     * Credit: aarthificial https://www.youtube.com/watch?v=uZmWgQ7cLNI.
     * See Optional.cs for the other side of this logic.
     */
    [CustomPropertyDrawer(typeof(Optional<>))]
    public class OptionalPropertyDrawer : PropertyDrawer
    {
        public override float GetPropertyHeight(SerializedProperty property, GUIContent label)
        {
            SerializedProperty valueProperty = property.FindPropertyRelative("_value");
            float propertyHeight = EditorGUI.GetPropertyHeight(valueProperty);
            return propertyHeight;
        }

        public override void OnGUI(Rect position, SerializedProperty property, GUIContent label)
        {
            SerializedProperty valueProperty = property.FindPropertyRelative("_value");
            SerializedProperty enabledProperty = property.FindPropertyRelative("_enabled");
            const float widthAdjustment = 24.0f;

            EditorGUI.BeginProperty(position, label, property);
            position.width -= widthAdjustment;
            EditorGUI.BeginDisabledGroup(!enabledProperty.boolValue);
            EditorGUI.PropertyField(position, valueProperty, label, true);
            EditorGUI.EndDisabledGroup();

            position.x += position.width + widthAdjustment;
            position.width = position.height = EditorGUI.GetPropertyHeight(enabledProperty);
            position.x -= position.width;
            EditorGUI.PropertyField(position, enabledProperty, GUIContent.none);
            EditorGUI.EndProperty();
        }
    }
}
