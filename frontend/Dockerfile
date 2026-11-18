# build env
FROM node:26.13.1-alpine3.24
WORKDIR /app
COPY package*.json ./
RUN npm install
COPY . ./
RUN npm run build
CMD cp -r build/. /frontend_static
