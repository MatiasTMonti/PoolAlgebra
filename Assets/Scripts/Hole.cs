using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Hole : MonoBehaviour
{
    private Vector2 position;

    private void Start()
    {
        position = gameObject.transform.position;
    }

    public Vector2 GetPosition()
    {
        return position;
    }
}
