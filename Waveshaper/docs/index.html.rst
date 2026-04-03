<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8" />
  <meta name="viewport" content="width=device-width, initial-scale=1.0" />
  <meta name="description" content="Waveshaper DSP formula, code, notes and historical comments." />
  <meta name="author" content="kubu lee" />
  <title>Waveshaper :: kubu lee</title>

  <style>
    :root {
      --bg: #0b0f14;
      --bg2: #10161d;
      --panel: rgba(18, 24, 31, 0.9);
      --panel-2: rgba(24, 31, 40, 0.95);
      --text: #eaf2f8;
      --muted: #9fb0c0;
      --accent: #62d4ff;
      --accent-2: #7effc5;
      --border: #25303b;
      --code-bg: #0c1117;
      --shadow: 0 10px 40px rgba(0, 0, 0, 0.35);
      --radius: 18px;
    }

    * {
      box-sizing: border-box;
    }

    html {
      scroll-behavior: smooth;
    }

    body {
      margin: 0;
      font-family: Inter, ui-sans-serif, system-ui, -apple-system, BlinkMacSystemFont, "Segoe UI", sans-serif;
      background:
        radial-gradient(circle at top left, rgba(98, 212, 255, 0.08), transparent 35%),
        radial-gradient(circle at top right, rgba(126, 255, 197, 0.05), transparent 35%),
        linear-gradient(180deg, var(--bg) 0%, #0f141a 100%);
      color: var(--text);
      line-height: 1.7;
      min-height: 100vh;
    }

    a {
      color: var(--accent);
      text-decoration: none;
    }

    a:hover {
      text-decoration: underline;
    }

    .container {
      width: min(1100px, calc(100% - 2rem));
      margin: 0 auto;
      padding: 2rem 0 5rem;
    }

    .hero {
      position: relative;
      overflow: hidden;
      background: linear-gradient(135deg, rgba(98, 212, 255, 0.08), rgba(126, 255, 197, 0.04));
      border: 1px solid var(--border);
      border-radius: 28px;
      padding: 2.5rem;
      box-shadow: var(--shadow);
      margin-bottom: 2rem;
    }

    .hero::before {
      content: "";
      position: absolute;
      inset: 0;
      background:
        linear-gradient(120deg, transparent 0%, rgba(255,255,255,0.02) 30%, transparent 70%);
      pointer-events: none;
    }

    .badge {
      display: inline-flex;
      align-items: center;
      gap: 0.5rem;
      background: rgba(98, 212, 255, 0.08);
      border: 1px solid rgba(98, 212, 255, 0.2);
      color: var(--accent);
      padding: 0.45rem 0.8rem;
      border-radius: 999px;
      font-size: 0.85rem;
      font-weight: 600;
      margin-bottom: 1rem;
    }

    h1 {
      font-size: clamp(2.3rem, 4vw, 4rem);
      line-height: 1.05;
      margin: 0 0 0.8rem;
      letter-spacing: -0.03em;
    }

    h2 {
      font-size: clamp(1.35rem, 2vw, 1.8rem);
      margin: 0 0 1rem;
      letter-spacing: -0.02em;
    }

    h3 {
      margin: 0 0 0.8rem;
      font-size: 1.05rem;
      color: var(--accent-2);
    }

    p {
      margin: 0 0 1rem;
    }

    .lead {
      color: var(--muted);
      font-size: 1.08rem;
      max-width: 850px;
    }

    .meta {
      display: flex;
      flex-wrap: wrap;
      gap: 1rem;
      margin-top: 1.4rem;
      color: var(--muted);
      font-size: 0.95rem;
    }

    .grid {
      display: grid;
      grid-template-columns: 1.25fr 0.95fr;
      gap: 1.4rem;
      margin-top: 1.5rem;
    }

    @media (max-width: 900px) {
      .grid {
        grid-template-columns: 1fr;
      }
    }

    .card {
      background: var(--panel);
      border: 1px solid var(--border);
      border-radius: var(--radius);
      padding: 1.5rem;
      box-shadow: var(--shadow);
      backdrop-filter: blur(12px);
    }

    .note {
      border-left: 4px solid var(--accent);
      padding: 1rem 1rem 1rem 1.1rem;
      background: rgba(98, 212, 255, 0.06);
      border-radius: 12px;
      color: var(--text);
    }

    .formula {
      background: var(--code-bg);
      border: 1px solid var(--border);
      border-radius: 16px;
      padding: 1rem 1.1rem;
      overflow-x: auto;
      font-family: "JetBrains Mono", "Fira Code", Consolas, monospace;
      font-size: 0.96rem;
      color: #d8f2ff;
    }

    pre {
      margin: 0;
      overflow-x: auto;
      background: var(--code-bg);
      border: 1px solid var(--border);
      border-radius: 16px;
      padding: 1.1rem 1.2rem;
      box-shadow: inset 0 0 0 1px rgba(255,255,255,0.02);
    }

    code {
      font-family: "JetBrains Mono", "Fira Code", Consolas, monospace;
      font-size: 0.95rem;
      color: #d8f2ff;
    }

    .pill-row {
      display: flex;
      flex-wrap: wrap;
      gap: 0.7rem;
      margin-top: 1rem;
    }

    .pill {
      padding: 0.55rem 0.85rem;
      border-radius: 999px;
      background: rgba(255,255,255,0.04);
      border: 1px solid var(--border);
      color: var(--muted);
      font-size: 0.9rem;
    }

    .comment {
      background: var(--panel-2);
      border: 1px solid var(--border);
      border-radius: 16px;
      padding: 1rem 1rem 0.95rem;
      margin-top: 1rem;
    }

    .comment-meta {
      color: var(--muted);
      font-size: 0.9rem;
      margin-bottom: 0.6rem;
    }

    .footer {
      margin-top: 2rem;
      color: var(--muted);
      font-size: 0.92rem;
      text-align: center;
    }

    .copy-btn {
      margin-top: 0.9rem;
      background: linear-gradient(135deg, rgba(98, 212, 255, 0.14), rgba(126, 255, 197, 0.08));
      color: var(--text);
      border: 1px solid rgba(98, 212, 255, 0.25);
      border-radius: 12px;
      padding: 0.75rem 1rem;
      font: inherit;
      cursor: pointer;
      transition: transform 0.15s ease, opacity 0.15s ease;
    }

    .copy-btn:hover {
      transform: translateY(-1px);
      opacity: 0.95;
    }

    .copy-btn:active {
      transform: translateY(0);
    }

    .small {
      font-size: 0.92rem;
      color: var(--muted);
    }

    .player-wrap {
      margin-top: 1rem;
      border: 1px solid var(--border);
      border-radius: 18px;
      overflow: hidden;
      background: #0b1116;
    }

    #yumpuMagazineContainer {
      width: 100%;
      min-height: 520px;
    }

    @media (max-width: 768px) {
      #yumpuMagazineContainer {
        min-height: 380px;
      }
    }
  </style>

  <script src="https://players.yumpu.com/hub.js" defer></script>
</head>
<body>
  <main class="container">

    <section class="hero">
      <div class="badge">DSP / Audio / Waveshaping</div>
      <h1>Waveshaper</h1>
      <p class="lead">
        A nonlinear distortion transfer function using exponential curvature and input-dependent asymmetry.
        More interesting than a plain tanh, more expensive than it has any right to be.
      </p>

      <div class="meta">
        <span><strong>Author or source:</strong> maybe on IRC</span>
        <span><strong>Created:</strong> 2026-03-17 15:40:13</span>
        <span><strong>Status:</strong> cleaned and fixed</span>
      </div>

      <div class="pill-row">
        <span class="pill">Asymmetric saturation</span>
        <span class="pill">CPU-heavy reference curve</span>
        <span class="pill">Good candidate for LUT / approximation</span>
      </div>

      <div class="player-wrap">
        <div id="yumpuMagazineContainer"></div>
      </div>
    </section>

    <div class="grid">
      <section class="card">
        <h2>Overview</h2>
        <p>
          This function shapes an input signal into a soft, asymmetric nonlinear output.
          Unlike basic saturators such as <code>tanh(x)</code> or cubic clipping,
          this one bends dynamically based on the signal level itself.
        </p>
        <p>
          That makes it musically more interesting, especially for distortion, drive,
          character shaping, or “dirty analog-ish” DSP behavior.
        </p>

        <div class="note">
          <strong>Note:</strong> Multiply input by gain before processing.
        </div>
      </section>

      <section class="card">
        <h2>Why it matters</h2>
        <p class="small">
          This curve produces:
        </p>
        <div class="pill-row">
          <span class="pill">Soft clipping</span>
          <span class="pill">Level-dependent asymmetry</span>
          <span class="pill">Richer harmonics</span>
          <span class="pill">More “character” than generic saturation</span>
        </div>
      </section>
    </div>

    <section class="card" style="margin-top: 1.4rem;">
      <h2>Transfer Function</h2>
      <div class="formula">
<code>x = input × 0.686306

a = 1 + exp(-0.75 × sqrt(|x|))

output = (exp(x) - exp(-x × a)) / (exp(x) + exp(-x))</code>
      </div>
    </section>

    <section class="card" style="margin-top: 1.4rem;">
      <h2>Reference Code (C++)</h2>
      <pre><code>const double x = input * 0.686306;
const double a = 1 + exp(sqrt(fabs(x)) * -0.75);
output = (exp(x) - exp(-x * a)) / (exp(x) + exp(-x));</code></pre>
      <button class="copy-btn" onclick="copyCode('reference-code')">Copy code</button>

      <textarea id="reference-code" style="position:absolute;left:-9999px;top:-9999px;">
const double x = input * 0.686306;
const double a = 1 + exp(sqrt(fabs(x)) * -0.75);
output = (exp(x) - exp(-x * a)) / (exp(x) + exp(-x));
      </textarea>
    </section>

    <section class="card" style="margin-top: 1.4rem;">
      <h2>Optimized Real-Time Version</h2>
      <p>
        This is the better practical form. It removes one repeated exponential and is more suitable
        for real-time audio code.
      </p>

      <pre><code>inline float waveShaper(float input, float drive = 1.0f)
{
    float x = input * drive * 0.686306f;
    x = std::clamp(x, -6.0f, 6.0f);

    const float a = 1.0f + std::exp(-0.75f * std::sqrt(std::fabs(x)));

    const float ex   = std::exp(x);
    const float enx  = 1.0f / ex;        // exp(-x)
    const float enxa = std::exp(-x * a); // exp(-x*a)

    return (ex - enxa) / (ex + enx);
}</code></pre>
      <button class="copy-btn" onclick="copyCode('optimized-code')">Copy optimized code</button>

      <textarea id="optimized-code" style="position:absolute;left:-9999px;top:-9999px;">
inline float waveShaper(float input, float drive = 1.0f)
{
    float x = input * drive * 0.686306f;
    x = std::clamp(x, -6.0f, 6.0f);

    const float a = 1.0f + std::exp(-0.75f * std::sqrt(std::fabs(x)));

    const float ex   = std::exp(x);
    const float enx  = 1.0f / ex;        // exp(-x)
    const float enxa = std::exp(-x * a); // exp(-x*a)

    return (ex - enxa) / (ex + enx);
}
      </textarea>
    </section>

    <section class="card" style="margin-top: 1.4rem;">
      <h2>Notes</h2>
      <div class="pill-row">
        <span class="pill">Uses exp()</span>
        <span class="pill">Uses sqrt()</span>
        <span class="pill">Asymmetric</span>
        <span class="pill">Potential DC drift</span>
        <span class="pill">Best with oversampling</span>
        <span class="pill">Good LUT candidate</span>
      </div>

      <p style="margin-top: 1rem;">
        If you’re using this in a plugin, you should seriously consider:
      </p>
      <ul>
        <li>oversampling</li>
        <li>DC blocking</li>
        <li>lookup table acceleration</li>
        <li>drive / mix / output controls</li>
      </ul>
    </section>

    <section class="card" style="margin-top: 1.4rem;">
      <h2>Historical Comments</h2>

      <div class="comment">
        <div class="comment-meta">2026-09-25 21:42:39 — archived comment</div>
        <p>
          You can use a Taylor series approximation for the exp, save time by realizing that
          exp(-x) = 1/exp(x), use Newton's method to calculate the sqrt with less precision...
          and if you use SIMD instructions, you can calculate several values in parallel.
        </p>
      </div>

      <div class="comment">
        <div class="comment-meta">2005-09-13 09:55:55 — ShadowHugger</div>
        <p>Use table lookup with interpolation.</p>
      </div>

      <div class="comment">
        <div class="comment-meta">2005-09-22 01:07:58 — decil</div>
        <p>
          IMHO, you can use <code>x - 0.15 * x² - 0.15 * x³</code> instead of this scary formula.
        </p>
      </div>

      <div class="comment">
        <div class="comment-meta">Today — archived reply</div>
        <p>
          A polynomial can be cheaper, but it changes the harmonic behavior and may introduce DC from
          even-order terms unless compensated.
        </p>
      </div>
    </section>

    <p class="footer">
      Cleaned and rebuilt properly — no duplicated HTML, no broken nesting, no cursed markup.
    </p>
  </main>

  <script>
    function copyCode(id) {
      const textarea = document.getElementById(id);
      textarea.select();
      textarea.setSelectionRange(0, 99999);
      document.execCommand("copy");
      alert("Code copied.");
    }

    window.addEventListener("load", function () {
      if (typeof Yumpu !== "undefined") {
        try {
          const myYumpu = new Yumpu();
          myYumpu.create_player("#yumpuMagazineContainer", "", {
            lang: "en",
            embed_id: "IBeUFAzLQRc4EsHD",
            embedded: true
          });
        } catch (e) {
          document.getElementById("yumpuMagazineContainer").innerHTML =
            '<div style="padding:2rem;color:#9fb0c0;">Yumpu player could not load.</div>';
        }
      } else {
        document.getElementById("yumpuMagazineContainer").innerHTML =
          '<div style="padding:2rem;color:#9fb0c0;">Yumpu script not available.</div>';
      }
    });
  </script>
</body>
</html>
