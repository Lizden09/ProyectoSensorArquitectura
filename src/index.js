const express = require('express'); 
const { PrismaClient } = require('@prisma/client');
require('dotenv').config();
const prisma = new PrismaClient();
const app = express();
const PORT = process.env.PORT;

app.use(express.json());

app.get('/sensor', async (req, res)=>{
    try{
        const users = await prisma.sensor.findMany();
        res.json(users);

    }catch(error){
        console.error('error buscando usuarios:', error);
        res.status(500).send('error del servidor')
    }
});

app.post('/sensor', async (req, res) => {
    const { nombre, correo, carnet, latitud, longitud, humedad, temperatura } = req.body;

    const nuevoSensor = await prisma.sensor.create({
        data: {
            nombre,
            correo,
            carnet,
            latitud,
            longitud,
            humedad,
            temperatura
        }
    });

    res.json(nuevoSensor);
});

app.listen(PORT, () => {
    console.log(`Server is running on port ${PORT}`);
});  