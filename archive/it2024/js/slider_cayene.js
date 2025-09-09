const imageList = [
  '../res/img/cayene_1.jpg',
  '../res/img/cayene_2.jpg',
  '../res/img/cayene_3.jpg',
  '../res/img/cayene_4.jpg',
  '../res/img/cayene_5.jpg',
  '../res/img/cayene_6.jpg',
  '../res/img/cayene_7.jpg',
  '../res/img/cayene_8.jpg',
  '../res/img/cayene_9.jpg',
  '../res/img/cayene_10.jpg',
  '../res/img/cayene_11.jpg',
  '../res/img/cayene_12.jpg',
  '../res/img/cayene_13.jpg',
  '../res/img/cayene_14.jpg',
  '../res/img/cayene_15.jpg',
  '../res/img/cayene_16.jpg',
  '../res/img/cayene_17.jpg',
  '../res/img/cayene_18.jpg',
  '../res/img/cayene_19.jpg',


]
const img = document.querySelector('.img')
const next = document.getElementById('next')
const previous = document.getElementById('previous')
var numImage = 0
img.src = imageList[numImage]

next.addEventListener('click', () => {
  numImage++
  if (numImage >= imageList.length) {
    numImage = 0
  }
  console.log(numImage >= imageList.length)
  console.log(numImage)
  img.src = imageList[numImage]
})
previous.addEventListener('click', () => {
  numImage--
  if (numImage < 0) {
    numImage = imageList.length - 1
  }
  console.log(numImage >= imageList.length)
  console.log(numImage)
  img.src = imageList[numImage]
})