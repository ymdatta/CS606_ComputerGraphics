#version 330

in vec4 vCol;
in vec3 vPos;
in vec3 lightPos;
flat in int lflag;
out vec4 FragColor;

void main()
{
      vec3 lightColor = vec3(1.0, 1.0, 1.0);
      if (lflag == 0) {

      float ambientStrength = 0.1;
      vec4 ambient = ambientStrength * vec4(lightColor, 1.0);

      vec3 norm = vec3(normalize(vCol));
      vec3 lightDir = normalize(lightPos - vPos);

      float diff = max(dot(norm, lightDir), 0.0);
      vec4 diffuse = diff * vec4(lightColor, 1.0f);

//      FragColor = (ambient + diffuse) * vec4(0, 0, 1.0, 1.0);
     FragColor = (ambient + diffuse) * vCol;
      }
      else {
      FragColor = vec4(lightColor, 1.0);
      }
      
}
