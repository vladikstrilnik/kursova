from flask import Flask, request, jsonify
from database import init_db, get_connection

app = Flask(__name__)

init_db()

@app.route("/api/water", methods=["POST"])
def add_data():

    data = request.json

    conn = get_connection()

    conn.execute(
        """
        INSERT INTO water_log(device, liters, flow)
        VALUES(?,?,?)
        """,
        (
            data["device"],
            data["liters"],
            data["flow"]
        )
    )

    conn.commit()
    conn.close()

    return jsonify({"status":"ok"})

@app.route("/api/water/latest")
def latest():

    conn = get_connection()

    row = conn.execute(
        """
        SELECT *
        FROM water_log
        ORDER BY id DESC
        LIMIT 1
        """
    ).fetchone()

    conn.close()

    if row is None:
        return jsonify({})

    return jsonify({
        "device":row[1],
        "liters":row[2],
        "flow":row[3],
        "time":row[4]
    })

@app.route("/api/stat")
def stat():

    conn = get_connection()

    row = conn.execute("""
        SELECT
        MAX(flow),
        MAX(liters)
        FROM water_log
    """).fetchone()

    conn.close()

    return jsonify({
        "max_flow":row[0],
        "total":row[1]
    })

if __name__ == "__main__":
    app.run(host="0.0.0.0", port=5000)
