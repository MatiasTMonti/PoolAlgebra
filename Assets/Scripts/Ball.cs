using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Ball : MonoBehaviour
{
    private Vector2 position;

    private void Update()
    {
        position = gameObject.transform.position;
    }

    public Vector2 GetPosition()
    {
        return position;
    }
}
