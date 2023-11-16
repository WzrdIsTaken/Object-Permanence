using System.Collections;
using UnityEngine;
using UnityEngine.AI;

namespace ObjectPermanence
{
    /**
     * A basic agent which just moves towards the assigned target.
     * If the target moves away, it moves back to its start position.
     */
    public class BasicNavMeshAgentComponent : MonoBehaviour
    {
        [SerializeField] private GameObject _target;
        [SerializeField] private NavMeshAgent _agent;

        [SerializeField] private float _maxRange;
        [SerializeField] private float _updateNavigationFrequency;

        private Vector3 _startPosition;
        private bool _started; // why isn't this a default property ): or maybe it is and I'm missing it...

        public BasicNavMeshAgentComponent()
        {
            _target = null;
            _agent = null;
            _maxRange = 0.0f;
            _updateNavigationFrequency = 3.0f;
            _startPosition = default;
            _started = false;
        }

        private void Start()
        {
            if (!_target)
            {
                var players1 = GameObject.FindObjectsOfType<PlayerHealthComponent>();
                GameObject[] players = new GameObject[players1.Length];
                for (int i = 0; i < players.Length; ++i)
                {
                    players[i] = players1[i].gameObject;
                }

                if (players.Length >= 1)
                {
                    if (players.Length != 1)
                    {
                        DebugManager.Instance.AssertFail(AssertLevel.Assert, DebugCategory.Level, $"Found {players.Length} players.");
                        foreach (GameObject player in players)
                        {
                            DebugManager.Instance.Log(LogLevel.Warning, DebugCategory.Level, $"{player.name}");
                        }
                    }
                    
                    DebugManager.Instance.Log(LogLevel.Info, DebugCategory.Level, $"Setting enemy target to {players[0].name}");
                    _target = players[0];
                }
            }

            _startPosition = transform.position;
            _started = true;

            StartCoroutine(NavigationLoop());
        }

        private void OnEnable()
        {
            if (_started)
            {
                StartCoroutine(NavigationLoop());
            }
        }

        private void OnDisable()
        {
            _agent.isStopped = true;
            StopAllCoroutines(); // could also hold a ref to NavigationLoop?
        }

        private IEnumerator NavigationLoop()
        {
            while (true)
            {
                Vector3 targetPosition = _target && _target.activeSelf ? _target.transform.position : Vector3.positiveInfinity;

                if (CheckDistance(targetPosition, _maxRange))
                {
                    SetDestination(targetPosition);
                }
                else
                {
                    if (!CheckDistance(_startPosition, NavigationConstants.Epsilon))
                    {
                        SetDestination(_startPosition);
                    }
                }

                yield return new WaitForSeconds(_updateNavigationFrequency);
            }
        }

        private bool CheckDistance(Vector3 targetPosition, float maxDistance)
        {
            float distance = Vector3.Distance(transform.position, targetPosition);
            return (distance < maxDistance);
        }

        private void SetDestination(Vector3 target)
        {
            if (_agent.isOnNavMesh)
            {
                DebugManager.Instance.Log(LogLevel.Info, DebugCategory.Level, $"{gameObject.name} agent moving to {target}");
                _agent.SetDestination(target);
            }
        }
    }
}
