import React, { useState } from "react";
import { useTags } from './index.js'
import api, { getErrorMessage } from '../api'

export default function useRecipes () {
  const [ subscriptions, setSubscriptions ] = useState([])
  const [ subscriptionsPage, setSubscriptionsPage ] = useState(1)
  const [ subscriptionsCount, setSubscriptionsCount ] = useState(0)

  const removeSubscription = ({ id, callback }) => {
    api
      .deleteSubscriptions({ author_id: id })
      .then(res => {
        const subscriptionsUpdated = subscriptions.filter(item => {
          return item.id !== id
        })
        setSubscriptions(subscriptionsUpdated)
        setSubscriptionsCount(subscriptionsCount - 1)
        callback && callback()
      })
      .catch(err => {
        alert(getErrorMessage(err))
      })
  }
  
  return {
    subscriptions,
    setSubscriptions,
    subscriptionsPage,
    setSubscriptionsPage,
    removeSubscription,
    subscriptionsCount,
    setSubscriptionsCount
  }
}
