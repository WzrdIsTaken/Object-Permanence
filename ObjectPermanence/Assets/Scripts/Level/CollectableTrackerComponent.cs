using TMPro;
using UnityEngine;

namespace ObjectPermanence
{
    /**
     * Just tracks how many cubes the player has collected
     */ 
    public class CollectableTrackerComponent : MonoBehaviour
    {
        [SerializeField] TMP_Text _scoreText;
        private int _collectableCount;
        private int _collectedCount;

        private const string _scoreTextContent = "Cubes Collected: ";
        private const string _scoreFormat = "/";

        public CollectableTrackerComponent()
        {
            _scoreText = null;
            _collectableCount = 0;
            _collectedCount = 0;
        }

        private void Start()
        {
            DebugManager.Instance.Assert(FindObjectsOfType<CollectableTrackerComponent>().Length == 1,
                AssertLevel.Assert, DebugCategory.Level, "There should only be one CollectableTrackerComponent per scene");

            _collectableCount = FindObjectsOfType<CollectableComponent>().Length;
            UpdateScoreText();
        }

        public void Collect()
        {
            ++_collectedCount;
            UpdateScoreText();

            if (_collectedCount == _collectableCount)
            {
                DebugManager.Instance.Log(LogLevel.Info, DebugCategory.Level, 
                    "The player has collected all of the collectables");

                // Some other win stuff?
            }
        }

        private void UpdateScoreText()
        {
            _scoreText.text =
                StringUtils.BuildString(_scoreTextContent, _collectedCount.ToString(), _scoreFormat, _collectableCount.ToString());
        }
    }
}