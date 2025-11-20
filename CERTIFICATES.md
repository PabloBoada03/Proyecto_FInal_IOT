# Protocolos TLS y Certificados

## Internet de las Cosas

###### Pablo Boada - Cristian Castañeda - Santiago Prieto

### Preguntas

- ¿Que es el protocolo TLS, cual es su importancia y que es un certificado en ese contexto?
- ¿A que riesgos se está expuesto si no se usa TLS?
- ¿Que es un CA (Certificate Authority)?
- ¿Que es una cadena de certificados y cual es la vigencia promedio de los eslabones de la cadena?
- ¿Que es un keystore y que es un certificate bundle?
- ¿Que es la autenticación mutua en el contexto de TLS?
- ¿Cómo se habilita la validación de certificados en el ESP32?
- Si el sketch necesita conectarse a múltiples dominios con certificados generados por CAs distintos, ¿que alternativas hay?
- ¿Cómo se puede obtener el certificado para un dominio?
- ¿A qué se hace referencia cuando se habla de llave publica y privada en el contexto de TLS?
- ¿Que pasará con el código cuando los certificados expiren?
- ¿Que teoría matemática es el fundamento de la criptografía moderna? ¿Cuales son las posibles implicaciones de la computación cuántica para los métodos de criptografía actuales?

### Respuestas

1. El protocolo TLS (Travel Layer Security) consiste en el cifrado de comunicaciones por medio de Internet para garantizar la seguridad y privacidad de las transmisiones. El protocolo asegura que la información transmitida no sea interceptada o modificada al verificar la autenticidad de un servidor, permitiendo establecer comunicación cifrada de extremo a extremo. En este contexto un certificado TLS autentica un endpoint o sitio web, asegurando que solo el usuario y el servidor puedan acceder a la información de forma directa.
2. Uno de los riesgos más comunes de no usar certificados TLS es un ataque Man-in-the-Middle, donde un intruso puede interceptar las comunicaciones de un usuario con un sitio web o servidor y manipular la comunicación o extraer datos sensibles. También se puede arriesgar a la suplantación de identidad por un atacante, que puede pretender ser un servidor y no tendría forma de autenticar su identidad, exponiendo al usuario a ataques de phishing o robo de datos.
3. Un Certificate Authority (CA) es una entidad que valida y emite certificados de confianza para sitios web, validando su identidad y permitiendo el uso de comunicaciones cifradas.
4. Una cadena de certificados se utiliza para verificar la vigencia y autenticidad de los diferentes certificados que la componen. La cadena inicia con el certificado de entidad final, que es único y le es asignado a un servidor o sitio web, al que le siguen certificados intermedios que actúan como capas de seguridad y redundancia entre el certificado de entidad final y el certificado raíz, el cual es un certificado de confianza emitido directamente por un CA y que tiene la confianza del sistema operativo o navegador. Normalmente, un certificado de entidad final dura de 365 a 398 días, y debe ser renovado después de este periodo, pero los eslabones intermedios y raíz tienen periodos de vida mucho más largos.
5. Un keystore es un contenedor o repositorio que almacena las llaves privadas y certificados públicos de un servidor o sitio web, el cual permite autenticar clientes o servidores mediante TLS y protege estas mismas para prevenir la exportación de estas claves privadas. Un certificate bundle es cualquier repositorio que almacena certificados de un servidor, si tiene llaves privadas se le llama keystore y si almacena certificados de CA se llaman truststore.
6. mTLS o la autenticación TLS mutua es el protocolo que verifica la identidad tanto del servido como del cliente, a diferencia del protocolo TLS normal, que solo verifica la autenticidad del servidor. Este tipo de protocolo verifica que ambos, el servidor y el cliente, tengan certificados y claves confiables y correspondientes. Este proceso genera el "handshake" que establece los procesos de cifrado y las claves de sesión a utilizar en la comunicación.
7. En el entorno de Arduino IDE se utiliza la biblioteca de WiFiClientSecure para configurar los certificados raíz del servidor. Creamos una instancia de cliente en la ESP32 al declarar un objeto tipo WiFiClientSecure y cargando el certificado raíz con la función client.setCACert(). una vez cargado, establecemos la conexión con la función client.connect(host, port) donde el host y el puerto refieren al servidor.
8. Para un gran número de CAs, se podría implementar una solución de tipo "in-memory" para la gestión de certificados. Puedes cargar todos los certificados de CAs y cargarlos durante la conexión del cliente (client.connect), o también puedes usar un servidor proxy en el que valides los certificados.
9.  Para conseguir un certificado TLS se utiliza un servicio generador de certificados de un proveedor, o se utilizan los certificados que otorga la solución de hosting o nube utilizados. Normalmente para entornos de producción se solicita un certificado a un proveedor como DigiCert o SSL.com, o se utilizan alternativas gratuitas como OpenSSL o Certbot para generar certificados auto-firmados para entornos locales o de desarrollo.
10. Las llaves utilizadas en el protocolo TLS hacen referencia a los métodos de cifrado y descifrado de la comunicación segura. Una llave pública se utiliza para cifrar los mensajes enviados y se comparten con los certificados mientras que la llave privada descifra los mensajes recibidos a través de  el protocolo, y nunca se comparten ni exponen, manteniéndolos secretos y en el control de su propietario.
11. Un sitio con certificados expirados puede mostrarse como sitios no confiables, donde los datos no estén cifrados y los deje expuestos a los riesgos descritos anteriormente. Ademas, puede fallar el código o interrumpir el servicio.
12. La teoría matemática de la factorización de números grandes y los problemas de logaritmo discreto son la base de la criptografía asimétrica moderna. Los ordenadores cuánticos pueden resolver los problemas matemáticos subyacentes a la criptografía clásica de manera mucho más eficiente y rapida, haciendo que la seguridad de estos algoritmos se vea reducida de manera significante. 