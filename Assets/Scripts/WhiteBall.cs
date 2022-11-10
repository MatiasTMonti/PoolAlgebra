using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class WhiteBall : MonoBehaviour
{
    [SerializeField] private float force;

    private Vector2 position;
    private float gravity;
    private float mass;
    private Vector2 acceleration;
    private Vector2 velocity;
    private float MU;
    private float NU;
    private float norma_V;
    private float N;
    private float friction;

    private bool hitted;

    [SerializeField] private GameObject wall;

    private void Start()
    {
        position = gameObject.transform.position;
        friction = 0.1f;
        gravity = 9.8f;
        mass = 156;
        acceleration = new Vector2(0, 0);
        velocity = new Vector2(300, 100);
        MU = 0.005f;
        NU = 1.51f;
        norma_V = 0;
        N = mass * gravity;
    }

    private void Update()
    {
        Movement();
    }

    private void Movement()
    {
        norma_V = Mathf.Pow((Mathf.Pow(velocity.x, 2) + Mathf.Pow(velocity.y, 2)), 1 / 2);

        if (!hitted)
        {
            acceleration.x = N * MU * (velocity.x / norma_V) - NU * velocity.x;
            acceleration.y = N * MU * (velocity.y / norma_V) - NU * velocity.y;
            hitted = true;
        }

        position.x += velocity.x * Time.time;
        position.y += velocity.y * Time.time;

        if (velocity.x > 0)
        {
            velocity.x += acceleration.x * Time.time;
        }
        if (velocity.y > 0)
        {
            velocity.y += acceleration.y * Time.time;
        }

        CheckFriction();
        WallCollision();
    }

    private void WallCollision()
    {
        if (position.y > wall.transform.position.y || position.y < 0)
        {
            velocity.y *= -1;
        }

        if (position.x > wall.transform.position.x || position.x < 0)
        {
            velocity.x *= -1;
        }
    }

    private void CheckFriction()
    {
        if (velocity.x > 0)
        {
            acceleration.x -= friction;
        }

        if (velocity.y > 0)
        {
            acceleration.y -= friction;
        }
    }


    public void SetWhiteBallForce(float force)
    {
        this.force = force;
    }

    public Vector2 GetPosition()
    {
        return position;
    }
}
